import os
import re
from src.shift_cipher import encrypt
from src.chi_square_attack import chi_square_attack
from src.brute_force_dictionary import load_words, dictionary_attack


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
            "CRYPTOGRAPHY,10",
        ]
        with open(TESTCASE_FILE, "w") as f:
            f.write("# Format: plaintext,key\n")
            for line in sample:
                f.write(line + "\n")


def format_result(plaintext, actual_key, ciphertext, chi_key, chi_plaintext, chi_score,
                  dict_key=None, dict_plaintext="", dict_score=None, idx=None):
    header = f"--- Testcase {idx} ---" if idx is not None else "--- Result ---"
    parts = [
        header,
        f"Plaintext : {plaintext}",
        f"Actual Key: {actual_key}",
        f"Ciphertext: {ciphertext}",
        "",
        "--- Chi-Square Attack ---",
        f"Predicted Key : {chi_key}",
        f"Plaintext     : {chi_plaintext}",
        f"Score         : {chi_score}",
        f"Chi-Square Correct? : {chi_key == actual_key}",
        "",
        "--- Dictionary Attack ---",
        f"Predicted Key : {dict_key}",
        f"Plaintext     : {dict_plaintext}",
        f"Score         : {dict_score}",
        f"Dictionary Correct? : {dict_key == actual_key if dict_key is not None else False}",
        "",
    ]
    return "\n".join(parts)


def run_case(plaintext, actual_key):
    ciphertext = encrypt(plaintext, actual_key)
    chi_key, chi_plaintext, chi_score = chi_square_attack(ciphertext)
    dict_words_path = os.path.join(BASE_DIR, "dictionary", "english_words.txt")
    if os.path.exists(dict_words_path):
        words = load_words(dict_words_path)
        dict_key, dict_plaintext, dict_score = dictionary_attack(ciphertext, words)
    else:
        dict_key = None
        dict_plaintext = ""
        dict_score = None

    return format_result(plaintext, actual_key, ciphertext, chi_key, chi_plaintext, chi_score,
                         dict_key, dict_plaintext, dict_score)


def choose_mode():
    print("Choose input mode:")
    print("  1) Terminal (single test)")
    print("  2) Testcases folder (batch)")
    choice = input("Enter 1 or 2 [1]: ").strip()
    if choice == "":
        return 1
    if choice not in {"1", "2"}:
        print("Invalid choice; defaulting to 1.")
        return 1
    return int(choice)


def main():
    ensure_dirs_and_sample()

    mode = choose_mode()

    if mode == 1:
        plaintext = input("Enter plaintext: ")
        try:
            actual_key = int(input("Enter key (0-25): "))
        except ValueError:
            print("Invalid key. Must be integer 0-25.")
            return

        result = run_case(plaintext, actual_key)
        print(result)

    else:
        if not os.path.exists(TESTCASE_FILE):
            print("No testcase file found; a sample was created.")

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

                out.write(run_case(plaintext, actual_key))
                out.write("\n")

        print(f"Wrote results to {output_path}")


if __name__ == "__main__":
    main()
