#!/usr/bin/env python3
"""Educational AES-CBC padding-oracle attack demonstration.

The attack class deliberately receives only an oracle callable; it never accepts,
stores, or reads the AES key.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Callable

from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes


BLOCK_SIZE = AES.block_size


def pkcs7_pad(data: bytes, block_size: int = BLOCK_SIZE) -> bytes:
    """Return *data* padded according to PKCS#7."""
    padding_length = block_size - (len(data) % block_size)
    return data + bytes([padding_length]) * padding_length


def pkcs7_unpad(data: bytes, block_size: int = BLOCK_SIZE) -> bytes:
    """Validate and remove PKCS#7 padding."""
    if not data or len(data) % block_size:
        raise ValueError("padded data must contain complete blocks")
    padding_length = data[-1]
    if not 1 <= padding_length <= block_size:
        raise ValueError("invalid PKCS#7 padding length")
    if data[-padding_length:] != bytes([padding_length]) * padding_length:
        raise ValueError("invalid PKCS#7 padding bytes")
    return data[:-padding_length]


class AesCbcPaddingOracle:
    """A deliberately vulnerable service that leaks padding validity only."""

    def __init__(self, key: bytes | None = None) -> None:
        self._key = key or get_random_bytes(BLOCK_SIZE)

    def encrypt(self, plaintext: bytes) -> tuple[bytes, bytes]:
        iv = get_random_bytes(BLOCK_SIZE)
        ciphertext = AES.new(self._key, AES.MODE_CBC, iv).encrypt(pkcs7_pad(plaintext))
        return iv, ciphertext

    def has_valid_padding(self, iv: bytes, ciphertext: bytes) -> bool:
        """The vulnerable endpoint: True/False reveals the padding result."""
        if len(iv) != BLOCK_SIZE or not ciphertext or len(ciphertext) % BLOCK_SIZE:
            return False
        padded_plaintext = AES.new(self._key, AES.MODE_CBC, iv).decrypt(ciphertext)
        try:
            pkcs7_unpad(padded_plaintext)
        except ValueError:
            return False
        return True


@dataclass
class AttackResult:
    plaintext: bytes
    oracle_queries: int


class PaddingOracleAttack:
    """Recover CBC plaintext using only a boolean padding oracle."""

    def __init__(self, oracle: Callable[[bytes, bytes], bool]) -> None:
        self._oracle = oracle
        self.oracle_queries = 0

    def _query(self, crafted_previous: bytes, target_block: bytes) -> bool:
        self.oracle_queries += 1
        # Sending two blocks makes target_block the last decrypted block.  Its
        # padding is therefore what the oracle validates.
        return self._oracle(crafted_previous, target_block)

    def _recover_block(self, previous_block: bytes, target_block: bytes) -> bytes:
        intermediate = bytearray(BLOCK_SIZE)  # AES_decrypt(target_block)
        crafted = bytearray(BLOCK_SIZE)

        for index in range(BLOCK_SIZE - 1, -1, -1):
            padding = BLOCK_SIZE - index
            for suffix_index in range(index + 1, BLOCK_SIZE):
                crafted[suffix_index] = intermediate[suffix_index] ^ padding

            found = False
            for guess in range(256):
                crafted[index] = guess
                if not self._query(bytes(crafted), target_block):
                    continue

                # For the final byte, unchanged valid multi-byte padding can
                # produce a false positive.  Flip the preceding byte: genuine
                # 0x01 padding remains valid, longer padding does not.
                if index == BLOCK_SIZE - 1:
                    check = bytearray(crafted)
                    check[index - 1] ^= 1
                    if not self._query(bytes(check), target_block):
                        continue

                intermediate[index] = guess ^ padding
                found = True
                break
            if not found:
                raise RuntimeError(f"oracle attack failed at byte {index}")

        return bytes(a ^ b for a, b in zip(intermediate, previous_block))

    def recover(self, iv: bytes, ciphertext: bytes) -> AttackResult:
        """Recover and unpad every plaintext block in *ciphertext*."""
        if len(iv) != BLOCK_SIZE or not ciphertext or len(ciphertext) % BLOCK_SIZE:
            raise ValueError("invalid AES-CBC IV or ciphertext")
        self.oracle_queries = 0
        blocks = [ciphertext[i : i + BLOCK_SIZE] for i in range(0, len(ciphertext), BLOCK_SIZE)]
        previous = iv
        recovered = bytearray()
        for block in blocks:
            recovered.extend(self._recover_block(previous, block))
            previous = block
        return AttackResult(pkcs7_unpad(bytes(recovered)), self.oracle_queries)


def main() -> None:
    message = b"Meet at the library at 7 PM. Bring the cryptography notes."
    service = AesCbcPaddingOracle()
    iv, ciphertext = service.encrypt(message)

    attack = PaddingOracleAttack(service.has_valid_padding)
    result = attack.recover(iv, ciphertext)
    print("Recovered plaintext:", result.plaintext.decode("utf-8"))
    print("Oracle queries:", result.oracle_queries)
    print("Verification:", "PASS" if result.plaintext == message else "FAIL")


if __name__ == "__main__":
    main()
