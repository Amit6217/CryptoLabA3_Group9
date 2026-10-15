# Cryptographic Prime Generation Using Miller–Rabin

An educational implementation of cryptographic prime generation using the Miller–Rabin probabilistic primality test.

The objective of this project is to generate a probable prime number of exactly 512 bits. The program generates random odd candidate integers, tests their primality using multiple bases, and generates new candidates whenever a candidate fails the test.

The final candidate is verified to have the required bit length, pass the Miller–Rabin primality test, and pass an independent primality check using a trusted library.

> **Python 3 required:** On systems where `python` refers to Python 2.7, use `python3` and `python3 -m pip`. This project requires Python 3.

## Objective

To implement and analyze cryptographic prime generation using the Miller–Rabin primality test.

The main objectives are:

- Generate random odd integers of the assigned 512-bit length.
- Apply the Miller–Rabin primality test using multiple bases.
- Reject composite candidates and generate new candidates until a probable prime is found.
- Verify that the generated prime has exactly 512 bits.
- Record the number of candidates tested, the number of Miller–Rabin rounds, and the total execution time.
- Independently verify the generated prime using a trusted primality-checking library.

## Setup and Run

From the repository root:

```bash
python3 -m pip install -r requirements.txt
python3 src/prime_generation.py
python3 -m unittest discover -s testcases -v
```

**Note:** These commands assume that the implementation is saved as `src/prime_generation.py` and the tests are stored in `testcases/`. Update the paths if your repository uses a different directory structure.

The program generates random candidate integers and tests them until it finds a probable prime of exactly 512 bits.

After successful generation, it displays the generated prime, its actual bit length, the number of candidates tested, the number of Miller–Rabin rounds, and the total execution time.

The exact prime and execution time vary between runs because the candidates are randomly generated.

## How It Works

### 1. Cryptographic Prime Generation

Large prime numbers are important in public-key cryptography, including RSA.

A prime number is an integer greater than 1 that has exactly two positive divisors: 1 and itself.

For example:

- 2 is prime.
- 7 is prime.
- 15 is composite because it is divisible by 3 and 5.

Cryptographic applications require much larger primes than these examples.

In this project, the objective is to generate a probable prime with exactly 512 bits.

### 2. Generating a 512-Bit Candidate

The program generates a random integer with exactly 512 bits and ensures that it is odd.

A 512-bit integer satisfies:

\[
2^{511} \leq n < 2^{512}
\]

Therefore, its binary representation contains exactly 512 bits.

The candidate is made odd because every even integer greater than 2 is composite.

The generated candidate must satisfy:

\[
n \geq 2^{511}
\]

\[
n < 2^{512}
\]

\[
n \bmod 2 = 1
\]

The program then applies the Miller–Rabin primality test.

If the candidate fails, a new random candidate is generated.

### 3. Miller–Rabin Primality Test

The Miller–Rabin test is a probabilistic algorithm used to determine whether an integer is prime or composite.

It is widely used when generating large cryptographic prime numbers because it is efficient for large integers.

First, express the candidate in the form:

\[
n-1 = 2^s \times d
\]

where:

- \(n\) is the candidate integer.
- \(s\) is a positive integer.
- \(d\) is an odd positive integer.

The test then uses one or more bases, commonly represented by \(a\).

For each selected base:

1. Compute:

   \[
   x = a^d \bmod n
   \]

2. If \(x = 1\) or \(x = n-1\), the candidate passes that round.

3. Otherwise, repeatedly square \(x\), taking the result modulo \(n\), for up to \(s-1\) iterations.

4. If a result becomes \(n-1\), the candidate passes that round.

5. If none of these conditions is satisfied, the candidate is composite.

A candidate that fails any round is rejected.

A candidate that passes all selected rounds is considered a probable prime.

**Important:** Passing Miller–Rabin does not constitute a mathematical proof of primality by itself. The confidence depends on the bases used and the assumptions about how they are selected.

### 4. Repeating the Process

The prime-generation process follows these steps:

1. Generate a random odd 512-bit candidate.
2. Apply the Miller–Rabin primality test.
3. If the candidate is composite, reject it.
4. Generate another random odd 512-bit candidate.
5. Repeat until a candidate passes all required rounds.
6. Verify the final candidate's bit length.
7. Verify the result using an independent primality-checking method.
8. Record the experimental results.

The final output is a probable prime of the required length that passes the specified verification checks.

## Tasks and Deliverables Overview

### Task 1: Understand Cryptographic Prime Generation

The first task is to understand why large prime numbers are required in cryptography.

Prime numbers play an important role in public-key cryptographic algorithms such as RSA.

Generating large primes requires an efficient method of testing whether randomly generated candidates are prime.

Testing every possible divisor is impractical for 512-bit numbers because the search space is extremely large.

The Miller–Rabin test provides an efficient probabilistic alternative.

### Task 2: Implement the Miller–Rabin Test

The implementation should:

- Accept an integer candidate.
- Handle small integers and obvious composite cases.
- Decompose \(n-1\) into \(2^s d\), where \(d\) is odd.
- Select the required test bases.
- Perform modular exponentiation.
- Check the conditions for passing each round.
- Identify composite candidates.
- Accept candidates that pass all specified rounds as probable primes.

Modular exponentiation computes:

\[
a^d \bmod n
\]

without needing to construct the potentially enormous value \(a^d\) in full.

This makes the test practical for large integers.

### Task 3: Generate a 512-Bit Probable Prime

The prime-generation function should:

1. Generate a random candidate with exactly 512 bits.
2. Ensure that the candidate is odd.
3. Apply Miller–Rabin using multiple bases.
4. Reject candidates that fail.
5. Continue generating and testing candidates until one passes.
6. Return the generated probable prime.

The program should count every candidate tested, including the final successful candidate.

It should also measure the total time taken to complete the generation process.

### Task 4: Verify the Generated Prime

The final number must satisfy all the following conditions.

**Bit-length verification**

The generated number must have exactly 512 bits.

In Python:

```python
assert prime.bit_length() == 512
```

**Odd-number verification**

The generated prime must be odd.

```python
assert prime % 2 == 1
```

**Miller–Rabin verification**

The final number must pass the specified Miller–Rabin rounds.

```python
assert miller_rabin(prime, bases)
```

Here, `miller_rabin` and `bases` represent the function and base collection used by the implementation. Adapt the names to match the actual code.

**Independent verification**

The result should also be checked using an independent primality-checking implementation or trusted library.

For example, the `sympy` library provides a `isprime()` function.

Install it using:

```bash
python3 -m pip install sympy
```

Then verify the result:

```python
from sympy import isprime

assert isprime(prime)

print("Independent primality verification: PASSED")
```

The variable `prime` must contain the generated integer.

Independent verification provides additional confidence that the generated result is prime. The exact guarantees depend on the verification method used.

## Experimental Results

The program should record the following experimental parameters.

| Parameter | Description |
|---|---|
| Assigned bit length | Required length of the prime: 512 bits |
| Generated prime candidate | Final generated probable prime in decimal representation |
| Actual bit length | Number of bits in the generated prime |
| Number of candidates tested | Total candidates tested before finding the final probable prime, including the successful candidate |
| Number of Miller–Rabin rounds | Number of bases tested for the final candidate |
| Total execution time | Time required to generate and verify the prime |

### Experimental Results Table

Fill in the following table using the actual output of your program.

| Parameter | Value |
|---|---|
| Assigned bit length | 512 bits |
| Generated prime candidate | `<Insert actual generated prime>` |
| Actual bit length of the candidate | `<Insert measured bit length>` |
| Number of candidates tested | `<Insert actual count>` |
| Number of Miller–Rabin rounds | `<Insert actual round count>` |
| Total execution time | `<Insert measured execution time>` |
| Miller–Rabin verification | PASS |
| Independent primality verification | PASS |

**Note:** Do not use invented values for the generated prime, candidate count, or execution time. These values should come from an actual execution of the implementation.

The generated prime may differ between runs because each execution uses newly generated random candidates.

## Analysis of Results

### 1. Assigned and Actual Bit Length

The assigned bit length is 512 bits.

The actual bit length is calculated using:

\[
\operatorname{bitlength}(n)=\lfloor \log_2(n)\rfloor+1
\]

For a 512-bit candidate, the following condition must hold:

\[
2^{511} \leq n < 2^{512}
\]

The generated prime is accepted only if its actual bit length is exactly 512.

### 2. Number of Candidates Tested

The number of candidates tested indicates how many random odd integers were examined before a probable prime was found.

Each candidate that fails the primality test is discarded.

The process continues until a candidate passes all the required Miller–Rabin rounds.

The candidate count may vary between executions.

### 3. Number of Miller–Rabin Rounds

Each Miller–Rabin round tests the candidate using a selected base.

More rounds generally increase confidence that a candidate is prime, assuming the bases are selected appropriately.

For a candidate that passes \(k\) independent, uniformly selected rounds, the standard upper bound on the probability that a composite candidate passes every round is:

\[
P(\text{false prime}) \leq 4^{-k}
\]

This bound applies to the standard Miller–Rabin test with independently and uniformly selected bases under the usual assumptions.

It should not be interpreted as a universal guarantee for arbitrary fixed bases or every possible base-selection strategy.

### 4. Total Execution Time

The execution time measures the time required to generate and test candidates until a probable prime is found.

It depends on factors such as:

- Processor speed.
- Random candidate generation.
- Number of candidates tested.
- Number of Miller–Rabin rounds.
- Efficiency of modular exponentiation.
- Additional verification performed by the program.

The actual execution time should be measured and recorded during the experiment.

## Security Recommendations

1. **Use a cryptographically secure random-number generator.** Generate candidates using Python's `secrets` module rather than the standard `random` module for cryptographic applications.

2. **Use sufficient primality testing.** Apply an appropriate number of Miller–Rabin rounds and select bases according to the requirements of the application.

3. **Verify the bit length.** Ensure that the generated number has exactly 512 bits before accepting it.

4. **Use independent verification.** Check the final result using a trusted primality-checking library or another suitable primality-testing method.

5. **Use established cryptographic libraries in production.** Educational implementations help demonstrate the algorithm, but production cryptographic systems should use well-reviewed cryptographic libraries and follow the relevant key-generation standards.

6. **Protect generated cryptographic material.** If generated primes are used to construct cryptographic keys, protect the resulting private key material and follow the relevant key-management requirements.

## Conclusion

This project demonstrates the generation of a 512-bit probable prime using the Miller–Rabin primality test.

Random odd candidates are generated and tested using multiple bases. Candidates that fail the test are rejected, and the process repeats until a probable prime is found.

The final result is checked for its exact bit length, verified against the specified Miller–Rabin test, and tested using an independent primality-checking method.

The experiment records the generated prime, its bit length, the number of candidates tested, the number of Miller–Rabin rounds, and the total execution time.

The project demonstrates how probabilistic primality testing provides an efficient method for generating large prime candidates for cryptographic applications.

## Project Files

The following is a suggested project structure. Adjust the filenames to match your actual repository.

```text
Cryptographic-Prime-Generation/
│
├── README.md
├── requirements.txt
│
├── src/
│   └── prime_generation.py
│
├── testcases/
│   └── test_prime_generation.py
│
└── outputs/
    └── results.txt
```

### File Descriptions

- `src/prime_generation.py` — Implements random candidate generation, the Miller–Rabin primality test, probable-prime generation, and verification.
- `testcases/test_prime_generation.py` — Contains unit tests for candidate generation, bit-length validation, Miller–Rabin testing, and prime verification.
- `requirements.txt` — Lists the external Python dependencies required by the project.
- `outputs/results.txt` — Stores experimental results, including the generated prime, actual bit length, candidate count, round count, execution time, and verification status.
- `README.md` — Explains the objective, implementation, algorithm, experimental procedure, results, and security recommendations.

## References

1. Miller–Rabin primality testing and probabilistic primality algorithms.
2. Python documentation: [Secrets — Generate secure random numbers](https://docs.python.org/3/library/secrets.html).
3. SymPy documentation: [Number Theory](https://docs.sympy.org/latest/modules/ntheory.html).
4. NIST Digital Signature Standard and relevant cryptographic key-generation guidance.