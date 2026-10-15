import unittest
from prime_generation import (
    generate_candidate,
    miller_rabin,
    BIT_LENGTH,
    ROUNDS,
)
from sympy import isprime


class TestPrimeGeneration(unittest.TestCase):

    def test_candidate_bit_length(self):
        candidate = generate_candidate(BIT_LENGTH)
        self.assertEqual(candidate.bit_length(), 512)

    def test_candidate_is_odd(self):
        candidate = generate_candidate(BIT_LENGTH)
        self.assertEqual(candidate % 2, 1)

    def test_small_prime(self):
        self.assertTrue(miller_rabin(101, ROUNDS))

    def test_composite_number(self):
        self.assertFalse(miller_rabin(221, ROUNDS))

    def test_two_is_prime(self):
        self.assertTrue(miller_rabin(2, ROUNDS))

    def test_even_number_is_composite(self):
        self.assertFalse(miller_rabin(100, ROUNDS))

    def test_independent_primality_check(self):
        self.assertTrue(isprime(101))
        self.assertFalse(isprime(221))


if __name__ == "__main__":
    unittest.main(verbosity=2)