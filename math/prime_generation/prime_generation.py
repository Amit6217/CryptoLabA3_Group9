import secrets
import time
from pathlib import Path
from sympy import isprime


BIT_LENGTH = 512
ROUNDS = 20


def generate_candidate(bits):
    """Generate a random odd integer with exactly the given bit length."""
    candidate = secrets.randbits(bits)
    candidate |= (1 << (bits - 1))  # Set highest bit
    candidate |= 1                  # Make candidate odd
    return candidate


def miller_rabin(n, rounds=20):
    """Test whether n is probably prime using Miller-Rabin."""

    if n < 2:
        return False

    if n in (2, 3):
        return True

    if n % 2 == 0:
        return False

    # Write n - 1 = 2^s * d, where d is odd
    d = n - 1
    s = 0

    while d % 2 == 0:
        d //= 2
        s += 1

    # Use randomly selected bases
    for _ in range(rounds):
        a = 2 + secrets.randbelow(n - 3)

        x = pow(a, d, n)

        if x == 1 or x == n - 1:
            continue

        for _ in range(s - 1):
            x = pow(x, 2, n)

            if x == n - 1:
                break
        else:
            return False

    return True


def generate_prime(bits, rounds):
    """Generate candidates until one passes Miller-Rabin."""

    candidates_tested = 0
    start_time = time.perf_counter()

    while True:
        candidate = generate_candidate(bits)
        candidates_tested += 1

        if miller_rabin(candidate, rounds):
            elapsed_time = time.perf_counter() - start_time
            return candidate, candidates_tested, elapsed_time


def save_results(prime, candidates_tested, elapsed_time):
    """Save experimental results in results.txt."""

    output_file = Path(__file__).resolve().parent / "results.txt"

    with open(output_file, "w", encoding="utf-8") as file:
        file.write("MILLER-RABIN PRIME GENERATION\n")
        file.write("=" * 50 + "\n\n")

        file.write(f"Assigned bit length: {BIT_LENGTH}\n")
        file.write(f"Generated prime candidate: {prime}\n")
        file.write(f"Actual bit length: {prime.bit_length()}\n")
        file.write(f"Number of candidates tested: {candidates_tested}\n")
        file.write(f"Number of Miller-Rabin rounds: {ROUNDS}\n")
        file.write(f"Total execution time: {elapsed_time:.6f} seconds\n")

        file.write("\nVERIFICATION\n")
        file.write("-" * 50 + "\n")
        file.write(f"Correct bit length: {prime.bit_length() == BIT_LENGTH}\n")
        file.write(f"Passed Miller-Rabin: {miller_rabin(prime, ROUNDS)}\n")
        file.write(f"SymPy primality verification: {isprime(prime)}\n")

    print(f"\nResults saved to: {output_file}")


def main():
    print("CRYPTOGRAPHIC PRIME GENERATION USING MILLER-RABIN")
    print("=" * 55)
    print("Generating a 512-bit probable prime...\n")

    prime, candidates_tested, elapsed_time = generate_prime(
        BIT_LENGTH, ROUNDS
    )

    # Verify the final candidate
    correct_length = prime.bit_length() == BIT_LENGTH
    passed_miller_rabin = miller_rabin(prime, ROUNDS)
    independently_verified = isprime(prime)

    print("EXPERIMENTAL RESULTS")
    print("-" * 55)
    print(f"Assigned bit length: {BIT_LENGTH}")
    print(f"Generated prime candidate: {prime}")
    print(f"Actual bit length: {prime.bit_length()}")
    print(f"Number of candidates tested: {candidates_tested}")
    print(f"Number of Miller-Rabin rounds: {ROUNDS}")
    print(f"Total execution time: {elapsed_time:.6f} seconds")

    print("\nVERIFICATION")
    print("-" * 55)
    print(f"Correct bit length: {correct_length}")
    print(f"Passed Miller-Rabin: {passed_miller_rabin}")
    print(f"SymPy primality verification: {independently_verified}")

    if correct_length and passed_miller_rabin and independently_verified:
        print("\nSUCCESS: All verification checks passed.")
        save_results(prime, candidates_tested, elapsed_time)
    else:
        print("\nERROR: Verification failed.")


if __name__ == "__main__":
    main()