# Monoalphabetic Substitution Cipher Cryptanalysis — Group 9

A C++ implementation of the Monoalphabetic Substitution Cipher and its cryptanalysis using frequency analysis, word pattern analysis, and iterative key recovery. Developed as part of Assignment 5 for the Cryptography Laboratory (22CPP307) course.

## How to Compile and Run

### Prerequisites

- g++ (GCC) or any C++ compiler supporting C++17
- No external libraries required

### Compile

```bash
cd attacks/substitution_cipher_attack
g++ -o substitution_cipher src/main.cpp src/cipher.cpp src/analysis.cpp src/utils.cpp src/substitution.cpp -std=c++17
```

### Run

```bash
./substitution_cipher
```

Enter a 26-letter substitution key when prompted (e.g., `QWERTYUIOPASDFGHJKLZXCVBNM`), then use the interactive cryptanalysis menu to break the cipher.

## Plaintext Source & Preprocessing

The plaintext is taken from **page 39** (Group 9 = 9 + 30) of *"Introduction to Modern Cryptography"* by Jonathan Katz and Yehuda Lindell. It is stored in `testcases/plaintext.txt`.

### Text Cleaning & Natural English Representation

The original page 39 from the textbook contained mathematical exercises featuring LaTeX and Unicode symbols (e.g., Greek $\Pi$, $\epsilon$, inequality symbols $\ge, \le$, set membership $\in$, and prime notation $A'$). 

**Why non-English / non-ASCII characters were replaced with English equivalents:**
1. **Preventing Information Leakage**: Classical monoalphabetic substitution ciphers operate strictly on the 26 letters of the English alphabet ($A$–$Z$). Non-alphabetic and non-ASCII characters pass through unmodified. If Greek letters like $\Pi$ or symbols like $\ge$ remained in the ciphertext, an adversary would instantly identify the exact formulas and context without any cryptanalysis.
2. **Frequency Distortion**: Math variables and standalone Greek letters skew letter frequency counts away from natural English distributions.
3. **Encoding Integrity**: Mathematical symbols cause encoding discrepancies across standard terminals and file streams.

By translating symbols into their clean English phonetic and conceptual equivalents (e.g., $\Pi \to$ `Pi`, $\epsilon \to$ `epsilon`, $\ge \to$ `greater than or equal to`), the text forms genuine, natural English sentences that provide an authentic testbed for statistical cryptanalysis.

- **Total characters**: 2,733
- **Total alphabetic letters**: 2,202
- **Non-ASCII characters**: 0

## Folder Structure

```text
substitution_cipher_attack/
├── src/
│   ├── main.cpp                   # Entry point with interactive cryptanalysis menu
│   ├── cipher.cpp                 # Encrypt, decrypt, key validation
│   ├── analysis.cpp               # Frequency, word, and pattern analysis
│   ├── substitution.cpp           # Apply substitution, display partial plaintext, verify
│   └── utils.cpp                  # File I/O utilities
├── include/
│   ├── cipher.h
│   ├── analysis.h
│   ├── substitution.h
│   └── utils.h
├── testcases/
│   └── plaintext.txt              # Cleaned plaintext from Katz & Lindell (page 39)
├── outputs/
│   ├── ciphertext.txt             # Generated ciphertext
│   └── recovered_plaintext.txt    # Recovered plaintext after cryptanalysis
├── screenshots/
├── reports/
│   └── Assignment_5_Report.pdf    # Lab report PDF
├── substitution_cipher            # Compiled binary (Linux)
├── substitution_cipher.exe        # Compiled binary (Windows)
└── README.md                      # This file
```

## Required Functions / Modules

| Function | File | Description |
|---|---|---|
| `frequency_analysis()` | analysis.cpp | Counts letter frequencies, sorts in descending order, calculates percentages, identifies most frequent letters |
| `word_frequency_analysis()` | analysis.cpp | Analyzes one-letter, two-letter, three-letter, and repeated words |
| `pattern_analysis()` | analysis.cpp | Extracts letter patterns (e.g., HELLO -> ABCCD), detects doubled letters, groups by pattern |
| `apply_substitution()` | substitution.cpp | Applies partial mapping to ciphertext, displaying '?' for unknown letters |
| `display_partial_plaintext()` | substitution.cpp | Shows ciphertext and partial plaintext side-by-side with mapping table |
| `verify_solution()` | substitution.cpp | Re-encrypts plaintext with recovered key, verifies exact match with ciphertext |

## Letter Frequency Analysis Results

On the generated ciphertext (2,202 letters analyzed):

| Rank | Cipher Letter | Count | Percentage | Plain Letter | Standard English % |
|------|--------------|-------|------------|--------------|-------------------|
| 1 | T | 317 | 14.40% | E | 12.70% |
| 2 | Z | 216 | 9.81% | T | 9.06% |
| 3 | Q | 166 | 7.54% | A | 8.17% |
| 4 | O | 157 | 7.13% | I | 6.97% |
| 5 | L | 149 | 6.77% | S | 6.33% |
| 6 | K | 146 | 6.63% | R | 5.99% |
| 7 | G | 139 | 6.31% | O | 7.51% |
| 8 | F | 132 | 5.99% | N | 6.75% |
| 9 | I | 122 | 5.54% | H | 6.09% |
| 10 | E | 94 | 4.27% | C | 2.78% |

## Cryptanalytic Decision Table (Lab Notebook)

The step-by-step decisions used to iteratively break the cipher and recover the substitution key:

| Step | Observation | Possible Substitution | Substitution Tested | Result | Decision |
|:---:|-------------|----------------------|---------------------|--------|:--------:|
| 1 | `T` occurs most frequently (317, 14.4%) | `T -> E` | `T -> E` | Word endings match natural English patterns | Good decision |
| 2 | `ZIT` is the top 3-letter word (27x); `Z` is 2nd in frequency (9.8%) | `Z -> T`, `I -> H` | `Z -> T`, `I -> H` | `ZIT` becomes `THE`; `I` fits 'H' frequency | Good decision |
| 3 | `Q` occurs 25 times as a standalone 1-letter word | `Q -> A` | `Q -> A` | Forms natural English indefinite article "A" | Good decision |
| 4 | `QFR` appears 10x starting with `A` (`A F R`) | `F -> N`, `R -> D` | `F -> N`, `R -> D` | `QFR` becomes `AND`; `F` count (6.0%) fits 'N' | Good decision |
| 5 | `OL` occurs 10x as a 2-letter word; `L` is 6.77% frequent | `O -> I`, `L -> S` | `O -> I`, `L -> S` | `OL` becomes `IS`; `OZ` becomes `IT` | Good decision |
| 6 | `GFT` ends with `NE` (`F->N, T->E`), appearing with numbers | `G -> O` | `G -> O` | `GFT` becomes `ONE`; `GY` becomes `OF` | Good decision |
| 7 | `YGK` has pattern `_ O _` and `GY` is `O _` | `Y -> F`, `K -> R` | `Y -> F`, `K -> R` | `YGK` becomes `FOR`; `GY` becomes `OF` | Good decision |
| 8 | `VIOEI` has structure `V H I E H` with `I=H, O=I` | `V -> W`, `E -> C` | `V -> W`, `E -> C` | Resolves word `WHICH` | Good decision |
| 9 | Title `HTKYTEZSN LTEKTZ TFEKNHZOGF` has `_ E R F E C T _ _` | `H -> P`, `N -> Y`, `S -> L` | `H -> P`, `N -> Y`, `S -> L` | Resolves `PERFECTLY SECRET ENCRYPTION` | Good decision |
| 10 | `DTLLQUT` has structure `D E S S A _ E` | `D -> M`, `U -> G` | `D -> M`, `U -> G` | Resolves word `MESSAGE` | Good decision |
| 11 | `WT` maps to `_ E` following `AND THEN LETTING THE KEY` | `W -> B` | `W -> B` | Resolves phrase `BE A UNIFORM STRING` | Good decision |
| 12 | `XFOYGKD` has structure `_ N I F O R M` | `X -> U` | `X -> U` | Resolves word `UNIFORM` | Good decision |
| 13 | `COUTFTKT` has structure `_ I G E N E R E` | `C -> V` | `C -> V` | Resolves cipher name `VIGENERE` | Good decision |
| 14 | `ATN` has `_ E Y`; `WACFXVLPGF` has `R A N D O M I _ E D` | `A -> K`, `M -> Z`, `B -> X` | `A -> K`, `M -> Z`, `B -> X` | Resolves `KEY`, `RANDOMIZED`, `EXERCISE` | Good decision |

## Key Recovery and Verification

- **Original Encryption Key**: `QWERTYUIOPASDFGHJKLZXCVBNM`
- **Recovered Key**: `QWERTYUIOPASDFGHJKLZXCVBNM`
- **Verification**: The `verify_solution()` function re-encrypts `testcases/plaintext.txt` using the recovered key and compares it byte-for-byte against `outputs/ciphertext.txt`. The match is **100% identical**, confirming successful cryptanalysis.

## Observations

1. **Frequency analysis provides the entry point**: Initial hypotheses for the top characters (`T -> E`, `Z -> T`, `Q -> A`) provide immediate traction.
2. **Word structure gives high confidence**: Single-letter words (`Q` $\to$ `A`) and high-frequency trigrams (`ZIT` $\to$ `THE`) provide rigid anchors that prevent compounding errors.
3. **Pattern analysis resolves entire words at once**: Pattern-matching words like `VIOEI` (which maps uniquely to `WHICH`) and `DTLLQUT` (`MESSAGE`) reveal multiple low-frequency letters simultaneously.
4. **Removing non-alphabetic artifacts is critical**: Real-world classical cryptanalysis assumes pure text; mathematical symbols and non-ASCII characters bypass the cipher and contaminate statistical metrics if not sanitized.
5. **Deterministic substitution preserves language entropy**: Even though the key space is $26! \approx 4 \times 10^{26}$, monoalphabetic substitution provides negligible security because it leaves character frequencies, bigrams, and grammatical patterns completely intact.

## Conclusion

Monoalphabetic ciphers illustrate the difference between large key spaces and actual cryptographic security. Brute force is infeasible ($26!$ operations), yet the cipher falls quickly to frequency and structural analysis in $O(n)$ time. To achieve true security, modern cryptographic algorithms employ Shannon's principles of **confusion** (complex relationship between key and ciphertext) and **diffusion** (spreading the plaintext statistics across the ciphertext).
