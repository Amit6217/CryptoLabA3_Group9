"""Generate 1 MiB samples from the available cryptographic random sources."""

from pathlib import Path
import os
import platform
import subprocess
import sys


OUTPUT_DIR = Path(__file__).resolve().parent
SIZE = 1024 * 1024


def generate_linux_urandom(destination: Path) -> None:
    with open("/dev/urandom", "rb") as source, destination.open("wb") as output:
        output.write(source.read(SIZE))


def generate_openssl(destination: Path) -> None:
    subprocess.run(
        ["openssl", "rand", "-out", str(destination), str(SIZE)],
        check=True,
    )


def generate_windows_cng(destination: Path) -> None:
    if os.name != "nt":
        raise OSError("Windows CNG is only available on Windows.")

    import ctypes

    bcrypt = ctypes.WinDLL("bcrypt.dll")
    random_bytes = bcrypt.BCryptGenRandom
    random_bytes.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_ulong, ctypes.c_ulong]
    random_bytes.restype = ctypes.c_long
    buffer = (ctypes.c_ubyte * SIZE)()
    # BCRYPT_USE_SYSTEM_PREFERRED_RNG = 0x00000002
    status = random_bytes(None, buffer, SIZE, 0x00000002)
    if status != 0:
        raise OSError(f"BCryptGenRandom failed with NTSTATUS 0x{status & 0xFFFFFFFF:08X}")
    destination.write_bytes(bytes(buffer))


def main() -> int:
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    generators = []
    if platform.system() == "Linux":
        generators.append(("Linux /dev/urandom", "linux_urandom.bin", generate_linux_urandom))
    if os.name == "nt":
        generators.append(("Windows CNG", "windows_cng.bin", generate_windows_cng))
    generators.append(("OpenSSL rand", "openssl_rand.bin", generate_openssl))

    for name, filename, generate in generators:
        try:
            generate(OUTPUT_DIR / filename)
            print(f"Generated {SIZE:,} bytes with {name}: {filename}")
        except (OSError, subprocess.CalledProcessError) as error:
            print(f"Skipped {name}: {error}", file=sys.stderr)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
