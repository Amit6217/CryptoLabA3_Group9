# Shift Cipher Cryptanalysis — Group 9

A Python-based cryptanalysis toolkit for breaking the Shift (Caesar) Cipher using two attack methods: **Chi-Square Statistical Analysis** and **Brute Force with Dictionary Scoring**. Developed as part of Assignment 4 for the Cryptography Laboratory (22CPP307) course.

## How to Run

### Prerequisites

- Python 3.x installed on your system
- No external packages are required

### Terminal Mode (Single Test)

```bash
cd attacks/shift_cipher_attack
python main.py
```

Select option `1`, then enter a plaintext and a key (0–25). The program encrypts the plaintext, runs both attacks on the ciphertext, and displays the predicted keys and decrypted texts.

### Batch Mode (Testcases File)

```bash
cd attacks/shift_cipher_attack
python main.py
```

Select option `2`. The program reads all testcases from `testcases/testcases.txt`, runs both attacks on each, and writes the results to `outputs/results.txt`.

### Testcase File Format

```
# Lines starting with # are comments
HELLO WORLD,3
ATTACK AT DAWN,5
```

Each line contains a plaintext followed by a comma and the shift key (0–25).

## Folder Structure

```text
shift_cipher_attack/
├── src/
│   ├── shift_cipher.py            # Shift cipher encrypt/decrypt
│   ├── chi_square_attack.py       # Chi-square statistical attack
│   └── brute_force_dictionary.py  # Brute-force dictionary scoring attack
├── dictionary/
│   └── english_words.txt          # English word list for dictionary attack
├── testcases/
│   └── testcases.txt              # 10 test cases (plaintext,key)
├── outputs/
│   └── results.txt                # Attack results for all test cases
├── screenshots/
├── reports/
│   └── Assignment_4_Report.pdf    # Lab report
├── main.py                        # Entry point (terminal & batch modes)
└── README.md                      # This file
```

## Algorithms

### Shift Cipher (Caesar Cipher)

The shift cipher encrypts each letter by shifting it forward by a fixed key `k` in the alphabet:

```
E(x) = (x + k) mod 26
D(y) = (y - k) mod 26
```

Non-alphabetic characters (spaces, punctuation) are preserved unchanged. The implementation handles both uppercase and lowercase letters.

### Attack 1 — Chi-Square Statistical Analysis

The chi-square attack exploits the known frequency distribution of letters in the English language. For each possible key (0–25):

1. Decrypt the ciphertext using the candidate key
2. Count the frequency of each letter in the decrypted text
3. Compute the chi-square statistic against expected English frequencies:

```
χ² = Σ (observed_i - expected_i)² / expected_i
```

The key that produces the **lowest χ² score** (closest match to English) is selected as the predicted key.

**Strengths:** Works well on longer texts where letter frequencies converge to English norms.
**Weaknesses:** May fail on short texts where statistical distribution is unreliable.

### Attack 2 — Brute Force with Dictionary Scoring

The dictionary attack uses a list of known English words. For each possible key (0–25):

1. Decrypt the ciphertext using the candidate key
2. Split the decrypted text into words
3. Count how many words appear in the English dictionary file

The key that produces the **highest dictionary match score** is selected as the predicted key.

**Strengths:** Effective even on short texts, as recognizing even one or two English words is sufficient.
**Weaknesses:** Depends on the quality and coverage of the dictionary file.

## Results

All 10 testcases were run in batch mode. Here is the summary:

| # | Plaintext | Actual Key | Chi-Square Key | Dict Key | χ² Correct? | Dict Correct? |
|---|-----------|-----------|----------------|----------|-------------|---------------|
| 1 | HELLO WORLD | 3 | 6 | 3 | ❌ | ✅ |
| 2 | ATTACK AT DAWN | 5 | 5 | 5 | ✅ | ✅ |
| 3 | CRYPTOGRAPHY IS FUN | 10 | 23 | 10 | ❌ | ✅ |
| 4 | THIS IS A SIMPLE TEST | 7 | 7 | 7 | ✅ | ✅ |
| 5 | THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG | 12 | 12 | 12 | ✅ | ✅ |
| 6 | MEET ME AT THE STATION | 4 | 4 | 4 | ✅ | ✅ |
| 7 | COMPUTER SECURITY IS IMPORTANT | 8 | 8 | 8 | ✅ | ✅ |
| 8 | INFORMATION SECURITY | 15 | 15 | 15 | ✅ | ✅ |
| 9 | SHIFT CIPHER IS EASY TO UNDERSTAND | 6 | 6 | 6 | ✅ | ✅ |
| 10 | THIS IS A CRYPTOGRAPHY LAB | 20 | 20 | 20 | ✅ | ✅ |

**Chi-Square Accuracy:** 8/10 (80%)
**Dictionary Accuracy:** 10/10 (100%)

## Failure Analysis

The chi-square attack failed on two testcases:

1. **HELLO WORLD (key=3):** Only 10 letters. With such a small sample, the observed letter frequencies do not resemble the expected English distribution for any key, leading to an incorrect prediction (key 6 instead of 3).

2. **CRYPTOGRAPHY IS FUN (key=10):** Only 16 letters. Again, the text is too short for the chi-square statistic to reliably distinguish the correct key from competing candidates (predicted key 23 instead of 10).

**Why it fails:** The chi-square test is a statistical method that assumes a sufficiently large sample size. With fewer than ~20 letters, the observed frequencies are dominated by random variation rather than the underlying English distribution.

**Suggested improvements:**
- Use a **weighted scoring approach** that combines chi-square with dictionary scoring for short texts
- Apply **trigram or bigram frequency analysis** in addition to single-letter frequencies
- Increase the dictionary size for more robust fallback detection

## Observations

1. The **dictionary attack** achieved 100% accuracy across all testcases, demonstrating its reliability for texts containing common English words.

2. The **chi-square attack** performed well on longer texts (≥20 characters) but struggled with shorter texts where the letter frequency distribution is not statistically representative.

3. Longer texts like "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG" (which uses all 26 letters) produced a higher chi-square score (109.3) even when correctly identified — this is because the pangram has a more uniform letter distribution than typical English text.

4. Both attacks are computationally efficient, requiring only 26 iterations (one per possible key) regardless of text length, making them O(26 × n) where n is the text length.

5. The two methods are complementary: chi-square excels at statistical pattern matching on longer texts, while dictionary scoring excels at semantic recognition even on short texts.

## Conclusion

This assignment demonstrated the practical cryptanalysis of the Shift Cipher using two distinct approaches. The key takeaways are:

- The Shift Cipher is inherently weak because its small key space (only 26 possible keys) makes brute-force attacks trivial.
- Statistical methods like chi-square analysis are powerful but require sufficient data to be reliable. They work best on ciphertexts of 50+ characters.
- Dictionary-based scoring provides a robust alternative that works even on very short texts, as long as the plaintext contains recognizable words.
- Combining multiple attack strategies produces the most reliable results, compensating for individual method weaknesses.
- Real-world ciphers must use much larger key spaces and more complex transformations to resist cryptanalysis.
