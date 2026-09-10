# Cryptanalysis of the Vigenere Cipher

This directory contains the implementation for the assignment **“Cryptanalysis
of Vigenere Cipher using Kasiski Examination and Frequency Analysis.”** The
program accepts ciphertext, estimates the Vigenere key length, recovers a
probable key, decrypts the message, and verifies the answer by encrypting the
recovered plaintext again.

The included `input/ciphertext.txt` is Ciphertext 1 from the assignment sheet
(for odd group numbers).

## Assignment requirements covered

| Requirement | Implementation |
| --- | --- |
| Preprocess ciphertext | Removes spaces, punctuation, digits, and other non-letters; normalizes letters to uppercase. |
| Estimate key length | Kasiski examination finds repeated 3-5 character patterns and factors their occurrence distances. Index of Coincidence (IC) selects the final length. |
| Divide ciphertext into groups | Creates one group for each position in the repeating key. |
| Frequency analysis | Prints an A-Z count and percentage table for every group. |
| Determine probable key | Uses chi-square comparison against English letter frequencies to identify a Caesar shift for every group. |
| Decrypt | Applies Vigenere decryption with the recovered key. |
| Verify | Encrypts the recovered plaintext again and compares it with the cleaned original ciphertext. |

## Directory structure

```text
vigenere_cryptanalysis/
├── input/
│   └── ciphertext.txt       # Default assignment ciphertext
├── main.cpp                 # Program entry point and analysis workflow
├── kasiski.h / kasiski.cpp  # Repeated-pattern, distance, factor, and Kasiski logic
├── frequency.h / frequency.cpp
│                            # IC, grouping, frequency tables, and key recovery
├── vigenere.h / vigenere.cpp
│                            # Cleaning, encryption, decryption, verification
└── README.md
```

## How the attack works

### 1. Preprocessing

`clean_ciphertext()` retains only alphabetic characters and converts them to
uppercase. This is important because the cipher operates on A-Z only; spaces
and line breaks must not alter the key position.

### 2. Kasiski examination

If the same plaintext sequence occurs more than once at the same point in a
repeating Vigenere key, it produces the same ciphertext sequence. The program:

1. Finds repeated sequences of length 3, 4, and 5.
2. Calculates the distances between each pair of occurrences.
3. Finds the factors of those distances.
4. Counts factor votes for potential key lengths 2 through 20.

Factors with more votes are stronger Kasiski candidates. Kasiski results alone
can include multiples of the true length or coincidental repetitions, so the
program uses IC as a second check.

### 3. Index of Coincidence

For a text with letter counts \(f_i\) and total length \(N\), the IC is:

\[
IC = \frac{\sum_{i=0}^{25} f_i(f_i - 1)}{N(N - 1)}
\]

English-like text usually has a higher IC (roughly 0.066) than uniformly
distributed text (roughly 0.038). For every candidate length from 1 to 20, the
ciphertext is split into that many groups and the program computes the average
group IC. The length with the highest average is selected.

### 4. Grouping and frequency analysis

For a key length \(m\), characters at positions `0, m, 2m, ...` form group 1;
positions `1, m+1, 2m+1, ...` form group 2; and so on. Each group has been
encrypted with one fixed Caesar shift, so it can be attacked independently.

The program prints the count and percentage of each letter A-Z in every group.

### 5. Recovering the key

For each group, all 26 possible Caesar shifts are tested. The program computes
a chi-square score against standard English letter frequencies:

\[
\chi^2 = \sum_{i=0}^{25}\frac{(O_i - E_i)^2}{E_i}
\]

where \(O_i\) is the observed count and \(E_i\) is the expected count. The
shift with the smallest score is chosen. Combining the selected shifts in group
order produces the probable Vigenere key.

### 6. Decryption and verification

With letters represented by `A = 0` through `Z = 25`, decryption uses:

\[
P_i = (C_i - K_{i \bmod m} + 26) \bmod 26
\]

The program then encrypts the recovered plaintext using the recovered key:

\[
C_i = (P_i + K_{i \bmod m}) \bmod 26
\]

It reports `PASS` only if this re-encrypted text exactly matches the cleaned
original ciphertext.

## Build and run

### Option 1: from this directory

```bash
cd classical/vigenere_cryptanalysis
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp kasiski.cpp frequency.cpp vigenere.cpp -o vigenere
./vigenere
```

### Option 2: from the repository root

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic \
  classical/vigenere_cryptanalysis/main.cpp \
  classical/vigenere_cryptanalysis/kasiski.cpp \
  classical/vigenere_cryptanalysis/frequency.cpp \
  classical/vigenere_cryptanalysis/vigenere.cpp \
  -o /tmp/vigenere

/tmp/vigenere classical/vigenere_cryptanalysis/input/ciphertext.txt
```

The program requires a C++17-compatible compiler such as `g++`.

## Analysing a different ciphertext

Supply the path to a text file containing ciphertext as the first command-line
argument:

```bash
./vigenere path/to/ciphertext.txt
```

The file may contain lowercase letters, spaces, punctuation, numbers, and line
breaks. They are removed or normalized during preprocessing. The ciphertext
should be long enough for repeated patterns and English frequency statistics to
be meaningful.

## Program output

The output is organized into these sections:

1. Ciphertext length after preprocessing.
2. Kasiski candidate key lengths and their factor-vote scores.
3. Average IC for every tested key length (1-20).
4. Selected key length and its average IC.
5. A-Z frequency table for each ciphertext group.
6. Recovered key and plaintext.
7. `PASS` or `FAIL` for the re-encryption verification.

## Verified result for the supplied input

Running the program with `input/ciphertext.txt` produces:

| Item | Result |
| --- | --- |
| Cleaned ciphertext length | 395 letters |
| Estimated key length | 14 |
| Recovered key | `AMBROISETHOMAS` |
| Verification | `PASS` |

The recovered plaintext, shown without spaces because preprocessing operates on
letters only, begins:

```text
DOYOUKNOWTHELANDWHERETHEORANGETREEBLOSSOMSTHECOUNTRYOFGOLDENFRUITS
ANDMARVELOUSROSES...
```

## Function reference

| Function | File | Purpose |
| --- | --- | --- |
| `clean_ciphertext()` | `vigenere.cpp` | Normalizes text to uppercase A-Z. |
| `find_repeated_patterns()` | `kasiski.cpp` | Finds repeated ciphertext sequences. |
| `calculate_distances()` | `kasiski.cpp` | Finds distances between pattern occurrences. |
| `find_factors()` | `kasiski.cpp` | Counts factors of Kasiski distances. |
| `kasiski_analysis()` | `kasiski.cpp` | Ranks candidate key lengths by factor votes. |
| `calculate_ic()` | `frequency.cpp` | Calculates the Index of Coincidence. |
| `split_into_groups()` | `frequency.cpp` | Separates ciphertext by key position. |
| `frequency_analysis()` | `frequency.cpp` | Counts the A-Z frequencies of a group. |
| `find_shift()` | `frequency.cpp` | Finds the best Caesar shift using chi-square scoring. |
| `find_key()` | `frequency.cpp` | Combines group shifts into a Vigenere key. |
| `vigenere_decrypt()` | `vigenere.cpp` | Decrypts ciphertext with a key. |
| `vigenere_encrypt()` | `vigenere.cpp` | Encrypts plaintext with a key. |
| `verify()` | `vigenere.cpp` | Checks whether re-encryption reproduces ciphertext. |

## Notes and limitations

- Kasiski examination and frequency analysis are statistical attacks. Very
  short ciphertexts can produce unreliable results.
- A key length that is a multiple of the real key length may also receive high
  Kasiski votes; IC helps distinguish candidates.
- The recovered plaintext is displayed without word spacing because spacing is
  removed from the ciphertext before cryptanalysis.
- The current program evaluates lengths 1 through 20. Increase that range in
  `main.cpp` if a longer key is expected.
