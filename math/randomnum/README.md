# Random Numbers

This generates 1 MiB samples and summarizes their byte and bit distributions, repeated patterns, and adjacent-byte correlation.

## Requirements

- Python 3.8 or newer
- OpenSSL command-line tool (`openssl rand`)
- Linux for the `/dev/urandom` sample
- Windows to generate the Windows CNG sample

## Run

From this directory:

```bash
python random_number_generator.py
python analyze_random.py
```

The generator creates `linux_urandom.bin` on Linux, `windows_cng.bin` on
Windows, and `openssl_rand.bin` when OpenSSL is available. The analyzer reads
all `.bin` samples in this directory and writes `results.txt`.

On Linux, Windows CNG cannot be run locally; run the generator on Windows to
produce that sample. The analysis is descriptive and does not prove that a
generator is cryptographically secure.
