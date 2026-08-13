import hashlib
import os
import subprocess
import random

# Vulnerability 1: Hardcoded Secret / Credential (Bandit B105)
SECRET_API_KEY = "sk_live_998877665544332211"
ADMIN_PASSWORD = "SuperSecretPassword123!"

def authenticate_user(user_input):
    # Vulnerability 2: Use of assert for security checking (Bandit B101)
    assert user_input == ADMIN_PASSWORD, "Authentication Failed!"
    print("[+] Access Granted")

def hash_user_data(data):
    # Vulnerability 3: Use of weak cryptographic hash MD5 (Bandit B303)
    return hashlib.md5(data.encode()).hexdigest()

def execute_system_command(user_filename):
    # Vulnerability 4: Unsafe shell command execution / Command Injection risk (Bandit B602)
    cmd = f"ls -la {user_filename}"
    subprocess.Popen(cmd, shell=True)

def generate_session_token():
    # Vulnerability 5: Standard pseudo-random generator unfit for crypto (Bandit B311)
    return str(random.randint(100000, 999999))

if __name__ == "__main__":
    print("Running Vulnerable Test Program...")
    authenticate_user("SuperSecretPassword123!")
    print("Hashed Data:", hash_user_data("sensitive_info"))
    generate_session_token()
