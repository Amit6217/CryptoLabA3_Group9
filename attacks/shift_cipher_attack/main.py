import os
import re
from src.shift_cipher import encrypt
from src.chi_square_attack import chi_square_attack


BASE_DIR = os.path.dirname(__file__)
TESTCASE_DIR = os.path.join(BASE_DIR, "testcases")
OUTPUT_DIR = os.path.join(BASE_DIR, "outputs")
TESTCASE_FILE = os.path.join(TESTCASE_DIR, "testcases.txt")


def ensure_dirs_and_sample():
    os.makedirs(TESTCASE_DIR, exist_ok=True)
    os.makedirs(OUTPUT_DIR, exist_ok=True)

    if not os.path.exists(TESTCASE_FILE):
        sample = [
            "HELLO WORLD,3",
            "ATTACK AT DAWN,5",
            "CRYPTOGRAPHY,10"
        ]
        with open(TESTCASE_FILE, "w") as f:
            f.write("# Format: plaintext, key (one per line)\n")
            for line in sample:
                f.write(line + "\n")
        print(f"Generated sample testcases at {TESTCASE_FILE}")


def run_case(plaintext, actual_key, out_handle=None, idx=None):
    ciphertext = encrypt(plaintext, actual_key)
    chi_key, chi_plaintext, chi_score = chi_square_attack(ciphertext)

    lines = []
    header = f"--- Testcase {idx} ---" if idx is not None else "--- Result ---"
    lines.append(header)
    lines.append(f"Plaintext : {plaintext}")
    lines.append(f"Actual Key: {actual_key}")
    lines.append(f"Ciphertext: {ciphertext}")
    lines.append("")
    lines.append("--- Chi-Square Attack ---")
    lines.append(f"Predicted Key : {chi_key}")
    lines.append(f"Plaintext     : {chi_plaintext}")
    lines.append(f"Score         : {chi_score}")
    lines.append(f"Chi-Square Correct? : {chi_key == actual_key}")
    lines.append("\n")

    output = "\n".join(lines)

    print(output)

    if out_handle:
        out_handle.write(output + "\n")


def main():
    ensure_dirs_and_sample()

    mode = input("Select mode ('terminal' or 'testcases') [terminal]: ").strip().lower()
    if mode == "":
        mode = "terminal"

    if mode == "terminal":
        plaintext = input("Enter plaintext: ")
        try:
            actual_key = int(input("Enter key (0-25): "))
        except ValueError:
            print("Invalid key. Must be an integer 0-25.")
            return

        run_case(plaintext, actual_key)

    elif mode == "testcases":
        if not os.path.exists(TESTCASE_FILE):
            print("No testcase file found; a sample was generated.")

        with open(TESTCASE_FILE) as f:
            lines = [l.strip() for l in f if l.strip() and not l.strip().startswith("#")]

        if not lines:
            print("No testcases to run.")
            return

        output_path = os.path.join(OUTPUT_DIR, "results.txt")
        with open(output_path, "w") as out:
            for i, line in enumerate(lines, start=1):
                parts = re.split(r"[,\t|]", line, maxsplit=1)
                if len(parts) < 2:
                    print(f"Skipping malformed line: {line}")
                    continue
                plaintext = parts[0].strip()
                try:
                    actual_key = int(parts[1].strip())
                except ValueError:
                    print(f"Skipping line with invalid key: {line}")
                    continue

                run_case(plaintext, actual_key, out_handle=out, idx=i)

        print(f"Wrote results to {output_path}")

    else:
        print("Unknown mode. Choose 'terminal' or 'testcases'.")


if __name__ == "__main__":
    main()