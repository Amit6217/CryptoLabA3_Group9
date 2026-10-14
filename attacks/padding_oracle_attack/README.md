# AES-CBC Padding Oracle Attack

An educational implementation of a padding-oracle attack against AES-CBC with PKCS#7 padding. It recovers plaintext without giving the attack code the AES key. The local `AesCbcPaddingOracle` service holds the key and leaks only whether a submitted ciphertext has valid padding.

> **Python 3 required:** On systems where `python` means Python 2.7, use `python3` and `python3 -m pip`. Python 2 cannot parse this program's type annotations.

## Setup and run

From the repository root:

```bash
python3 -m pip install -r requirements.txt
python3 attacks/padding_oracle_attack/src/padding_oracle_attack.py
python3 -m unittest discover -s attacks/padding_oracle_attack/testcases -v
```

The program generates a new AES key, IV, and ciphertext for each run, then prints the recovered plaintext and measured oracle-query count. The exact count varies because each byte is found by trying candidate values from `0` through `255`.

## How it works

In CBC mode, plaintext block `P_i` is computed as:

```text
P_i = AES-Decrypt(C_i) XOR C_(i-1)
```

For the first block, `C_(i-1)` is the IV. The attacker changes the byte in the previous ciphertext block (or IV) that corresponds to the byte being recovered. A `True` response tells the attacker that the resulting last plaintext bytes form valid PKCS#7 padding. Starting with padding value `0x01` at the right edge and moving left, this reveals the intermediate AES-decryption value; XORing it with the original previous block yields the real plaintext.

The attack sends a two-block ciphertext for every guess so the block under attack is always the final block checked by the oracle. The `PaddingOracleAttack` class accepts only an oracle function, so it cannot read or use the secret AES key.


## Tasks & Deliverables Overview

### Task 1: Understand the Attack
- **AES-CBC:** Encrypts 16-byte blocks with the recurrence $C_i = E_K(P_i \oplus C_{i-1})$. Decryption computes $P_i = D_K(C_i) \oplus C_{i-1}$, where $I_i = D_K(C_i)$ is the intermediate state.
- **PKCS#7 Padding:** Appends $N$ bytes each equal to $N$ ($1 \le N \le 16$) to align data to the block boundary.
- **Padding Oracle:** An oracle that exposes whether a given $(IV, C)$ decrypts to a valid PKCS#7 padded plaintext, creating a 1-bit side-channel.

### Task 2: Implement the Attack
- The `PaddingOracleAttack` class modifies the crafted previous block byte-by-byte from right to left (`range(15, -1, -1)`).
- Dynamically calculates suffixes to match padding $pad = 16 - k$.
- Disambiguates candidate byte false positives for $0x01$ by toggling byte 14 (`crafted[14] ^= 1`).
- Sequentially decrypts all blocks and strips PKCS#7 padding without key access.

### Task 3: Analyze the Attack
- **Why modifying previous block affects target plaintext:** Because $P_i[k] = I_i[k] \oplus C_{i-1}[k]$, the attacker directly controls $P_i[k]$ bit-by-bit through $C_{i-1}[k]$ while $I_i[k]$ remains constant.
- **Query Complexity:**
  - Average per byte: 128 queries (worst case 256).
  - Average per block: $16 \times 128 = 2,048$ queries.
  - 4-block message (64 bytes): Expected $\approx 8,192$ queries. Measured $\approx 8,047$ queries.

### Task 4: Security Recommendations & Prevention
1. **Adopt Authenticated Encryption (AEAD):** Use modern AEAD modes such as **AES-GCM** or **ChaCha20-Poly1305** that verify cryptographic integrity before decryption.
2. **Encrypt-then-MAC:** If CBC mode is strictly required, apply HMAC-SHA256 over $IV \mathbin{\Vert} C$ and verify the tag in constant time prior to decryption.
3. **Uniform Error Handling:** Never return distinct error messages or allow timing discrepancies between padding failures and authentication failures.

## Files

- `src/padding_oracle_attack.py` — vulnerable AES-CBC oracle, attack implementation, and demo.
- `testcases/test_padding_oracle_attack.py` — recovery and PKCS#7 validation unit tests.
- `outputs/results.txt` — recorded execution results, query counts, and verification status across test cases.
- `reports/Assignment_7_Report.pdf` — complete formal lab report for Assignment 7.
- `PADDING_ORACLE_EXPLAINED.md` — beginner-friendly, visual walkthrough explaining encryption, PKCS#7 padding, and the attack mechanics with real numbers.
