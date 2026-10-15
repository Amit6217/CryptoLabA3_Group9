"""Tests for the padding-oracle demonstration."""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "src"))

from padding_oracle_attack import AesCbcPaddingOracle, PaddingOracleAttack, pkcs7_pad, pkcs7_unpad


class PaddingOracleAttackTests(unittest.TestCase):
    def test_recovers_messages_of_different_lengths(self) -> None:
        service = AesCbcPaddingOracle(bytes(range(16)))
        for message in (b"short", b"exactly sixteen!!", b"This message spans more than one AES block."):
            with self.subTest(message=message):
                iv, ciphertext = service.encrypt(message)
                result = PaddingOracleAttack(service.has_valid_padding).recover(iv, ciphertext)
                self.assertEqual(result.plaintext, message)
                self.assertGreater(result.oracle_queries, 0)

    def test_pkcs7_validation(self) -> None:
        self.assertEqual(pkcs7_unpad(pkcs7_pad(b"data")), b"data")
        with self.assertRaises(ValueError):
            pkcs7_unpad(b"bad\x02\x03")


if __name__ == "__main__":
    unittest.main()
