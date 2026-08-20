# Password Manager — Group 9

A console-based password manager application developed as part of Assignment 3 for the Cryptography Laboratory (22CPP307) course. The application demonstrates core credential management functionalities with intentionally embedded security vulnerabilities for SAST analysis.

## How to Run

### Prerequisites

- Python 3.x installed on your system
- No external packages are required

### Steps

1. **Navigate to the source directory:**

   ```bash
   cd secure_application/src
   ```

2. **Run the password manager:**

   ```bash
   python password_manager.py
   ```

3. **Login with the default credentials:**

   ```
   Username: admin
   Password: admin123
   ```

4. **Use the menu to manage passwords:**

   ```
   ========== MENU ==========
   1. Add Password
   2. View Passwords
   3. Search Password
   4. Delete Password
   5. Exit
   ```

   - Select option `1` to add a new entry (website, username, password)
   - Select option `2` to view all saved entries
   - Select option `3` to search by website name
   - Select option `4` to delete by website name
   - Select option `5` to exit the application

### Running the Bandit SAST Scan

To reproduce the security scan:

```bash
bandit -r secure_application/src/password_manager.py
```

To save the report:

```bash
bandit -r secure_application/src/password_manager.py -f txt -o secure_application/sast/bandit_report.txt
bandit -r secure_application/src/password_manager.py -f json -o secure_application/reports/bandit_report.json
```

## Features

- **Login** — Authenticate before accessing stored credentials
- **Add Password** — Save website, username, and password
- **View Passwords** — Display all saved entries
- **Search Password** — Look up credentials by website name
- **Delete Password** — Remove an entry by website name

All credentials are stored in plaintext inside `outputs/passwords.txt` as required by the assignment. Encryption will be introduced in future assignments.

## Folder Structure

```text
secure_application/
├── src/
│   └── password_manager.py      # Main application source code
├── outputs/
│   └── passwords.txt            # Runtime plaintext password storage
├── sast/
│   └── bandit_report.txt        # Bandit SAST report (text format)
├── reports/
│   └── bandit_report.json       # Bandit SAST report (JSON format)
├── testcases/
│   └── testcases.txt            # 9 documented test cases
├── screenshots/
│   └── Screenshot *.png         # Execution screenshot
└── README.md                    # This file
```

## Intentional Vulnerabilities

Three security vulnerabilities were intentionally embedded from the assigned list for SAST analysis:

### 1. Hardcoded Credentials (Lines 4–5)

Admin username and password are hardcoded directly in the source code as plain string constants, making them visible to anyone with access to the codebase.

```python
ADMIN_USERNAME = "admin"
ADMIN_PASSWORD = "admin123"
```

**How to verify:** Open `src/password_manager.py` and read lines 4–5.

### 2. Information Leakage (Lines 25–26)

When a user enters incorrect login credentials, the application prints the valid username and password in debug output, exposing sensitive information.

```python
print("Debug: Valid username =", ADMIN_USERNAME)
print("Debug: Valid password =", ADMIN_PASSWORD)
```

**How to verify:** Run the application, enter any wrong username/password, and observe the output:

```
Login failed!
Debug: Valid username = admin
Debug: Valid password = admin123
```

### 3. Insecure Storage (Lines 42–43)

Passwords are written to a plaintext file (`outputs/passwords.txt`) without any encryption or hashing, allowing anyone with file access to read all stored credentials.

```python
with open(STORAGE_FILE, "a") as file:
    file.write(f"{website},{username},{password}\n")
```

**How to verify:** Add a password, then open `outputs/passwords.txt` — all data is in plaintext:

```
gmail.com,pooja@gmail.com,pooja123
```

## SAST Analysis

Bandit was used to perform static security analysis on the password manager.

| Vulnerability | Detected? | Bandit ID | Reason |
|---------------|-----------|-----------|--------|
| Hardcoded Credentials | ✅ Yes | B105 (CWE-259) | Pattern matches variable names like `PASSWORD` assigned to string literals |
| Information Leakage | ❌ No | — | Logic-level flaw; Bandit cannot understand what data is sensitive |
| Insecure Storage | ❌ No | — | Design-level flaw; writing to a file is syntactically normal Python |

## What to Understand from This Application

### 1. SAST Tools Have Limits

Bandit detected the hardcoded password because it matches a **known code pattern** (variable named `PASSWORD` assigned to a string). But it completely missed the information leakage and insecure storage vulnerabilities because those are **logic-level and design-level flaws** — they require understanding *what* the code does, not just *how* it is written.

### 2. Zero Findings ≠ Secure Code

Just because a SAST tool reports zero or few issues does not mean the application is secure. In this case, two out of three critical vulnerabilities went completely undetected.

### 3. Security Requires Multiple Layers

A comprehensive security strategy must combine several approaches:

| Approach | Catches |
|----------|---------|
| **SAST** (Static Analysis) | Code patterns — hardcoded secrets, weak hashes, unsafe imports |
| **Manual Code Review** | Logic flaws — information leakage, authentication bypasses |
| **DAST** (Dynamic Testing) | Runtime vulnerabilities — injection attacks, session issues |
| **Threat Modeling** | Design weaknesses — insecure storage, missing encryption |

### 4. Plaintext Storage Is Never Acceptable

In a real-world application, passwords must be **hashed** (e.g., using bcrypt or Argon2), credentials should come from **environment variables** or secure vaults, and debug output must **never expose sensitive data**. These fixes are planned for future assignments when cryptographic algorithms are introduced.

## Test Cases

Nine test cases are documented in `testcases/testcases.txt`:

| Test Case | Description | Type |
|-----------|-------------|------|
| TC01 | Login with valid credentials | Functional |
| TC02 | Login with invalid credentials | Functional |
| TC03 | Add a new password entry | Functional |
| TC04 | View all saved passwords | Functional |
| TC05 | Search for a specific entry | Functional |
| TC06 | Delete a password entry | Functional |
| TC07 | Verify hardcoded credentials in source | Vulnerability |
| TC08 | Verify information leakage on failed login | Vulnerability |
| TC09 | Verify plaintext storage in passwords.txt | Vulnerability |
