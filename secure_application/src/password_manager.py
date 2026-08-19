import os

# Vulnerability 1: Hardcoded Credentials
ADMIN_USERNAME = "admin"
ADMIN_PASSWORD = "admin123"

# File used to store passwords
PASSWORD_FILE = "../outputs/passwords.txt"

def login():
    print("\n========== LOGIN ==========")

    username = input("Enter username: ")
    password = input("Enter password: ")

    if username == ADMIN_USERNAME and password == ADMIN_PASSWORD:
        print("Login successful!")
        return True

    print("Login failed!")

    # Vulnerability 2: Information Leakage
    # The application reveals the correct credentials.
    print("Debug: Valid username =", ADMIN_USERNAME)
    print("Debug: Valid password =", ADMIN_PASSWORD)

    return False

def add_password():
    print("\n========== ADD PASSWORD ==========")

    website = input("Enter website: ")
    username = input("Enter username: ")
    password = input("Enter password: ")

    # Create outputs directory if it does not exist
    os.makedirs("../outputs", exist_ok=True)

    # Vulnerability 3: Insecure Storage
    # Password is stored directly in plaintext.
    with open(PASSWORD_FILE, "a") as file:
        file.write(f"{website},{username},{password}\n")

    print("Password saved successfully!")

def view_passwords():
    print("\n========== SAVED PASSWORDS ==========")

    if not os.path.exists(PASSWORD_FILE):
        print("No passwords found.")
        return

    with open(PASSWORD_FILE, "r") as file:
        lines = file.readlines()

    if not lines:
        print("No passwords found.")
        return

    for line in lines:
        website, username, password = line.strip().split(",", 2)

        print("\nWebsite :", website)
        print("Username:", username)
        print("Password:", password)

def search_password():
    print("\n========== SEARCH PASSWORD ==========")

    search = input("Enter website: ")

    if not os.path.exists(PASSWORD_FILE):
        print("No passwords found.")
        return

    found = False

    with open(PASSWORD_FILE, "r") as file:
        for line in file:
            website, username, password = line.strip().split(",", 2)

            if website.lower() == search.lower():
                print("\nWebsite :", website)
                print("Username:", username)
                print("Password:", password)

                found = True

    if not found:
        print("Website not found.")


def delete_password():
    print("\n========== DELETE PASSWORD ==========")

    website_to_delete = input("Enter website: ")

    if not os.path.exists(PASSWORD_FILE):
        print("No passwords found.")
        return

    with open(PASSWORD_FILE, "r") as file:
        lines = file.readlines()

    deleted = False

    with open(PASSWORD_FILE, "w") as file:

        for line in lines:
            website, username, password = line.strip().split(",", 2)

            if website.lower() == website_to_delete.lower():
                deleted = True
                continue

            file.write(line)

    if deleted:
        print("Password deleted successfully.")
    else:
        print("Website not found.")


def main():

    print("====================================")
    print("          PASSWORD MANAGER")
    print("             GROUP 9")
    print("====================================")

    # Authentication
    if not login():
        return

    while True:

        print("\n========== MENU ==========")
        print("1. Add Password")
        print("2. View Passwords")
        print("3. Search Password")
        print("4. Delete Password")
        print("5. Exit")

        choice = input("Enter your choice: ")

        if choice == "1":
            add_password()

        elif choice == "2":
            view_passwords()

        elif choice == "3":
            search_password()

        elif choice == "4":
            delete_password()

        elif choice == "5":
            print("Exiting Password Manager...")
            break

        else:
            print("Invalid choice.")


if __name__ == "__main__":
    main()