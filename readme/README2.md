# CryptoLabA3_Group9

A Python-based **Cryptography Laboratory toolkit** developed for the Cryptography Laboratory course. The project is being developed incrementally to implement cryptographic algorithms, cryptanalysis techniques, mathematical utilities, file analysis, and security testing.

## Instructor

**Dr. Meenakshi Tripathi**

## Team Members

* **2024UCP1562** — [AMIT KUMAR](https://github.com/Amit6217)
* **2024UCP1717** — [GUTTULA POOJA](https://github.com/GUTTULAPOOJA)

## Project Structure

```text
CryptoLabA3_Group09/
├── classical/              # Classical cryptography algorithms
├── attacks/                # Cryptanalysis and attack techniques
├── math/                   # Mathematical utilities
├── modern/                 # Modern cryptographic algorithms
├── analysis/               # Text and file analysis
├── datasets/               # Sample input files
├── outputs/                # Generated analysis results
├── docs/                   # Documentation
├── tests/                  # Testing and security experiments
├── utils/                  # Utility modules
├── logs/                   # Application logs
├── main.py                 # Main CLI application
├── requirements.txt        # Python dependencies
└── README.md
```

## Current Features

* Interactive command-line interface
* File and text analysis
* Character, word, line, and unique-character counting
* Letter-frequency analysis
* Activity logging
* Sample datasets for analysis
* Security testing using **Bandit**
* Vulnerable-code examples for static security analysis

## Bandit Security Analysis

The repository includes a small vulnerable test program in `tests/vulnerable_test.py` to demonstrate common Python security issues.

Bandit analysis results are stored in:

```text
outputs/bandit_results.txt
outputs/bandit_results.json
```

The analysis identifies issues including hardcoded credentials, unsafe `assert` usage, weak MD5 hashing, shell command execution, and insecure random number generation.

## Running the Project

Clone the repository:

```bash
git clone https://github.com/Amit6217/CryptoLabA3_Group9.git
cd CryptoLabA3_Group9
```

Run the application:

```bash
python main.py
```

The CLI currently provides options for encryption, decryption, attacks, file analysis, and exiting the application.

## Security Testing

Install Bandit if required:

```bash
pip install bandit
```
Run the security scan:
```bash
bandit -r .
```
For JSON output:
```bash
bandit -r . -f json -o outputs/bandit_results.json
```

## Progress

* Project structure and Git repository setup
* Interactive CLI and logging
* File analysis module
* Sample dataset integration
* Modular code organization
* Bandit-based security analysis

## Future Work

* Classical cipher implementations
* Modern cryptographic algorithms
* Cryptanalysis attacks
* Mathematical cryptography utilities
* Improved testing and documentation

