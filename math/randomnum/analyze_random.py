"""Summarize byte and bit distributions and simple repetition/correlation."""

from __future__ import annotations

from collections import Counter
from pathlib import Path
import math


DATA_DIR = Path(__file__).resolve().parent
RESULTS_FILE = DATA_DIR / "results.txt"
BIT_COUNTS = tuple(bin(value).count("1") for value in range(256))


def correlation(values: list[int]) -> float | None:
    """Pearson correlation between adjacent values, or None if undefined."""
    if len(values) < 2:
        return None
    left, right = values[:-1], values[1:]
    left_mean = sum(left) / len(left)
    right_mean = sum(right) / len(right)
    numerator = sum((a - left_mean) * (b - right_mean) for a, b in zip(left, right))
    left_sum = sum((a - left_mean) ** 2 for a in left)
    right_sum = sum((b - right_mean) ** 2 for b in right)
    denominator = math.sqrt(left_sum * right_sum)
    return numerator / denominator if denominator else None


def longest_equal_byte_run(data: bytes) -> int:
    longest = current = 0
    previous = None
    for value in data:
        current = current + 1 if value == previous else 1
        longest = max(longest, current)
        previous = value
    return longest


def analyze(path: Path) -> str:
    data = path.read_bytes()
    if not data:
        return f"{path.name}: empty file; skipped\n"

    frequencies = Counter(data)
    one_bits = sum(BIT_COUNTS[value] for value in data)
    repeated_4_byte_windows = len(data) - 3 - len({data[i:i + 4] for i in range(len(data) - 3)})
    serial_corr = correlation(list(data))

    lines = [
        f"Source: {path.name}",
        f"Bytes analyzed: {len(data):,}",
        f"Distinct byte values: {len(frequencies)} of 256",
        f"Byte counts: min={min(frequencies.values()):,}, max={max(frequencies.values()):,}",
        f"Bit balance: ones={one_bits:,} ({one_bits / (len(data) * 8):.5%}), "
        f"zeros={len(data) * 8 - one_bits:,} ({1 - one_bits / (len(data) * 8):.5%})",
        f"Repeated 4-byte windows beyond first occurrences: {repeated_4_byte_windows:,}",
        f"Longest run of the same byte: {longest_equal_byte_run(data)}",
        f"Adjacent-byte Pearson correlation: {serial_corr:.6f}" if serial_corr is not None
        else "Adjacent-byte Pearson correlation: undefined",
        "",
    ]
    return "\n".join(lines)


def main() -> None:
    samples = sorted(DATA_DIR.glob("*.bin"))
    sections = [
        "Random Number Assignment — Tasks 1 and 2",
        "Descriptive checks only; these results do not establish cryptographic security.",
        "",
    ]
    if not samples:
        sections.append("No .bin samples found. Run random_number_assignment.py first.\n")
    else:
        sections.extend(analyze(sample) for sample in samples)
    RESULTS_FILE.write_text("\n".join(sections), encoding="utf-8")
    print(f"Analysis written to {RESULTS_FILE}")


if __name__ == "__main__":
    main()
