# CryptoLabA3_Group9

A Python-based cryptography toolkit developed for the Cryptography Laboratory (22CPP307) course. The project is built incrementally across lab assignments, evolving from a foundational CLI framework into a security analysis and application development platform.

## Instructor

**Dr. Meenakshi Tripathi**

## Team Members

| Roll Number | Name | GitHub |
|---|---|---|
| 2024UCP1562 | AMIT KUMAR | [Amit6217](https://github.com/Amit6217) |
| 2024UCP1717 | GUTTULA POOJA | [GUTTULAPOOJA](https://github.com/GUTTULAPOOJA) |

## Repository Structure

```text
CryptoLabA3_Group9/
├── classical/
├── modern/
├── attacks/
├── math/
├── analysis/
├── datasets/
├── outputs/
├── docs/
├── tests/
├── utils/
├── logs/
├── secure_application/
│   ├── src/
│   │   └── password_manager.py
│   ├── sast/
│   ├── reports/
│   ├── testcases/
│   ├── screenshots/
│   ├── outputs/
│   └── README.md
├── main.py
├── requirements.txt
└── README.md
```

## Getting Started

**Clone and run:**

```bash
git clone https://github.com/Amit6217/CryptoLabA3_Group9.git
cd CryptoLabA3_Group9
pip install -r requirements.txt
python main.py
```

**Run the Password Manager:**

```bash
cd secure_application/src
python password_manager.py
```

---

## Project Progress

### Assignment 1 — Project Foundation

The first assignment focused on building the foundation of the CryptoLab toolkit. We created the Git repository, set up the modular folder structure, and developed an interactive menu-driven command-line interface with options for Encrypt, Decrypt, Attack, Analyze, and Exit.

A file analysis module was implemented that reads text files from the `datasets/` folder and computes character count, word count, line count, unique characters, and letter frequency (a–z). Five sample text files were added to the datasets folder for testing and future use.

An automatic activity logging system was built that records every menu selection with a timestamp into `logs/activity.log`. The entire codebase was organized into modular packages (`analysis/`, `utils/`) for maintainability.

---

### Assignment 2 — Static Application Security Testing (SAST)

In the second assignment, we installed and configured **Bandit** — a Python SAST tool assigned to Group 9 — on the lab system (Ubuntu 20.04, Python 3.8.10).

We created a deliberately vulnerable test program (`tests/vulnerable_test.py`) containing five security issues:

- Hardcoded credentials (B105)
- Use of `assert` for authentication (B101)
- Weak MD5 hashing (B303)
- Shell command injection via `subprocess.Popen` with `shell=True` (B602)
- Insecure random number generation for security tokens (B311)

Bandit successfully identified all vulnerabilities, producing 7 findings across Low, Medium, and High severity levels. Reports were saved in both text and JSON formats under `outputs/`. The complete terminal session was captured in `sast_lab_log.txt` using the Linux `script` command.

---

### Assignment 3 — Secure Application Development

For the third assignment, we developed a **console-based Password Manager** application inside the `secure_application/` directory. The application assigned to Group 9 stores website credentials — usernames, website names, and passwords — in plaintext, as cryptography will be introduced in later assignments.

**Features implemented:**

- User login with authentication
- Add, view, search, and delete password entries
- Persistent storage in a plaintext file

**Three intentional vulnerabilities were embedded from the assigned list:**

1. **Hardcoded Credentials** — Admin username and password are hardcoded directly in the source code
2. **Information Leakage** — On failed login, the application reveals the valid credentials in debug output
3. **Insecure Storage** — Passwords are saved in plaintext without any encryption

Bandit was run against the password manager and detected the hardcoded credentials (B105). The other two vulnerabilities — information leakage and insecure storage — are logic-level flaws that Bandit cannot detect through pattern matching, highlighting a key limitation of static analysis tools. Nine test cases were documented covering both functional behavior and vulnerability verification.

---

## Future Work

- Classical cipher implementations (Caesar, Vigenère, Playfair)
- Modern cryptographic algorithms (DES, AES, RSA)
- Cryptanalysis attacks (brute force, frequency analysis, known-plaintext)
- Mathematical utilities (modular arithmetic, prime generation, extended Euclidean algorithm)
