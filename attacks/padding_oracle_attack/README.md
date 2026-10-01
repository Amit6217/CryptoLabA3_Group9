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

## Security recommendation

Do not expose distinct padding errors or timing differences. Prefer authenticated encryption such as AES-GCM or ChaCha20-Poly1305. If CBC must be used, authenticate the IV and ciphertext with an encrypt-then-MAC construction and verify the MAC before attempting decryption; return one uniform error response for all failures.

## Files

- `src/padding_oracle_attack.py` — vulnerable AES-CBC oracle, attack implementation, and demo.
- `testcases/test_padding_oracle_attack.py` — recovery and PKCS#7 validation tests.
