# Vigenère Cipher Cryptanalysis — Group 9

A C++-based cryptanalysis toolkit for breaking the Vigenère Cipher using **Kasiski Examination** and **Frequency Analysis with Chi-Square Scoring**. Developed as part of Assignment 6 for the Cryptography Laboratory (22CPP307) course.

## How to Run

### Prerequisites

- A C++17-compatible compiler (e.g., `g++`)
- No external libraries are required

### Steps

1. **Navigate to the source directory:**

   ```bash
   cd classical/vigenere_cryptanalysis
   ```

2. **Compile the program:**

   ```bash
   g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp kasiski.cpp frequency.cpp vigenere.cpp -o vigenere
   ```

3. **Run with the default ciphertext:**

   ```bash
   ./vigenere
   ```

   The program reads the ciphertext from `input/ciphertext.txt`, performs the full cryptanalysis, and displays the results.

4. **Run with a custom ciphertext:**

   ```bash
   ./vigenere path/to/ciphertext.txt
   ```

   The input file may contain lowercase letters, spaces, punctuation, numbers, and line breaks — all non-alphabetic characters are removed during preprocessing.

## Folder Structure

```text
vigenere_cryptanalysis/
├── input/
│   └── ciphertext.txt          # Assignment ciphertext (Ciphertext 1, odd groups)
├── output/
├── main.cpp                    # Entry point and analysis workflow
├── kasiski.h                   # Kasiski function declarations
├── kasiski.cpp                 # Repeated-pattern, distance, factor, and Kasiski logic
├── frequency.h                 # Frequency analysis function declarations
├── frequency.cpp               # IC, grouping, frequency tables, and key recovery
├── vigenere.h                  # Vigenère function declarations
├── vigenere.cpp                # Cleaning, encryption, decryption, verification
└── README.md                   # This file
```

## Algorithms

### Vigenère Cipher

The Vigenère cipher encrypts each letter using a repeating keyword. Each letter of the key provides a different Caesar shift:

```
E(P_i) = (P_i + K_(i mod m)) mod 26
D(C_i) = (C_i - K_(i mod m) + 26) mod 26
```

where `P_i` is the plaintext letter, `C_i` is the ciphertext letter, `K` is the key, and `m` is the key length. Non-alphabetic characters are removed before encryption.

### Step 1 — Preprocessing

`clean_ciphertext()` retains only alphabetic characters and converts them to uppercase. Spaces, digits, and punctuation are discarded because the cipher operates strictly on A–Z.

### Step 2 — Kasiski Examination (Key Length Estimation)

The Kasiski examination exploits the fact that identical plaintext sequences aligned at the same key position produce identical ciphertext sequences. The program:

1. **`find_repeated_patterns()`** — Scans for repeated sequences of length 3, 4, and 5 in the ciphertext
2. **`calculate_distances()`** — Computes the distance between each pair of occurrences of a repeated pattern
3. **`find_factors()`** — Finds all factors of each distance
4. **`kasiski_analysis()`** — Counts factor votes for potential key lengths 2 through 20 and ranks them by score

The key length is likely a factor that appears frequently across many patterns.

**Strengths:** Effective on sufficiently long ciphertexts with natural-language plaintext, where repeated trigrams are common.
**Weaknesses:** Can produce multiples of the true key length, and short ciphertexts may not contain enough repeated patterns.

### Step 3 — Index of Coincidence (IC) Confirmation

Kasiski alone may suggest multiple candidates, so the program uses IC as a second check. For each candidate key length from 1 to 20:

1. **`split_into_groups()`** — Divides the ciphertext into groups by key position (e.g., for key length `m`, group 1 = positions 0, m, 2m, …)
2. **`calculate_ic()`** — Computes the Index of Coincidence for each group:

```
IC = Σ f_i(f_i - 1) / N(N - 1)
```

where `f_i` is the count of letter `i` and `N` is the group length. English text has IC ≈ 0.066; random text has IC ≈ 0.038. The key length that produces the highest average IC across all groups is selected.

### Step 4 — Frequency Analysis and Key Recovery

Once the key length is determined, each group is an independent Caesar cipher. For each group:

1. **`frequency_analysis()`** — Counts the frequency of each letter A–Z and displays it as a table with percentages
2. **`find_shift()`** — Tests all 26 possible Caesar shifts, computing a chi-square score against expected English letter frequencies:

```
χ² = Σ (observed_i - expected_i)² / expected_i
```

   The shift producing the **lowest χ² score** (closest match to English) is selected.

3. **`find_key()`** — Combines the selected shifts from all groups to form the full Vigenère key

### Step 5 — Decryption and Verification

1. **`vigenere_decrypt()`** — Decrypts the ciphertext using the recovered key
2. **`vigenere_encrypt()`** — Re-encrypts the recovered plaintext using the same key
3. **`verify()`** — Checks that the re-encrypted text exactly matches the original cleaned ciphertext, reporting `PASS` or `FAIL`

## Results

Running the program on the assigned ciphertext (`input/ciphertext.txt` — Ciphertext 1 for odd group numbers):

| Item | Result |
|------|--------|
| Cleaned ciphertext length | 395 letters |
| Estimated key length | 14 (average IC = 0.0644) |
| Recovered key | `AMBROISETHOMAS` |
| Verification | `PASS` ✅ |

### Top Kasiski Candidates

| Key Length | Factor Votes |
|-----------|-------------|
| 2 | 24 |
| 7 | 22 |
| 14 | 22 |
| 3 | 16 |
| 6 | 15 |

Key length 14 was selected by IC confirmation (0.0644 ≈ English IC of 0.066), despite lengths 2 and 7 having comparable or higher Kasiski votes. Note that 7 is a factor of 14, which is expected — multiples of the true key length often score highly.

### Recovered Plaintext

The recovered plaintext (shown without spaces, as preprocessing removes them):

```
DO YOU KNOW THE LAND WHERE THE ORANGE TREE BLOSSOMS THE COUNTRY OF
GOLDEN FRUITS AND MARVELOUS ROSES WHERE THE BREEZE IS SOFTER AND
BIRDS LIGHTER WHERE BEES GATHER POLLEN IN EVERY SEASON AND WHERE
SHINES AND SMILES LIKE A GIFT FROM GOD AN ETERNAL SPRINGTIME UNDER
AN EVER BLUE SKY ALAS BUT I CANNOT FOLLOW YOU TO THAT HAPPY SHORE
FROM WHICH FATE HAS EXILED ME THERE IT IS THERE THAT I SHOULD LIKE
TO LIVE TO LOVE TO LOVE AND TO DIE IT IS THERE THAT I SHOULD LIKE
TO LIVE IT IS THERE YES THERE
```

This is from the aria *"Connais-tu le pays"* in Ambroise Thomas's opera *Mignon*.

## Observations

1. The **Kasiski examination** correctly identified 14 as a strong candidate (22 factor votes), tied with length 7. Since 7 divides 14, both receiving high votes is expected — the IC step is essential to distinguish the true key length from its factors.

2. The **Index of Coincidence** decisively selected length 14 (IC = 0.0644), which is close to the English IC of 0.066. All other lengths had IC values between 0.038–0.051, firmly in the random/polyalphabetic range.

3. The **chi-square scoring** correctly identified all 14 individual Caesar shifts on the first attempt, producing the key `AMBROISETHOMAS` without manual correction.

4. The **two-stage approach** (Kasiski → IC) is more robust than either method alone. Kasiski provides a shortlist of plausible key lengths, and IC selects the one whose grouping best resembles monoalphabetic English.

5. A ciphertext length of 395 letters is sufficient for this attack. Each of the 14 groups contains ~28 letters, which is enough for chi-square frequency analysis to work reliably.

6. The **re-encryption verification** confirms the result is exact — not an approximation. The recovered key and plaintext are mathematically proven correct.

## Conclusion

This assignment demonstrated the practical cryptanalysis of the Vigenère Cipher using two classical techniques. The key takeaways are:

- The Vigenère cipher, once considered "le chiffre indéchiffrable," is vulnerable to statistical cryptanalysis when the ciphertext is sufficiently long relative to the key.
- **Kasiski examination** exploits repeated patterns caused by the periodic key to estimate the key length, but may suggest multiples or factors of the true length.
- **Index of Coincidence** provides a statistical confirmation step that reliably distinguishes the correct key length from false candidates.
- **Chi-square frequency analysis** reduces each group to an independent Caesar cipher problem, enabling efficient key recovery.
- Combining multiple cryptanalytic techniques (Kasiski + IC + chi-square) produces robust results that can be verified through re-encryption.
- Modern ciphers avoid periodicity and letter-frequency leakage to resist these classical attacks.
