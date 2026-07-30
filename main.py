import sys

def display_menu():
    """Displays the main CLI menu for CryptoLabX."""
    print("\n" + "=" * 40)
    print("        CryptoLabX - Main Menu        ")
    print("=" * 40)
    print("1. Encrypt")
    print("2. Decrypt")
    print("3. Attack")
    print("4. Analyze")
    print("5. Exit")
    print("=" * 40)

def main():
    while True:
        display_menu()
        choice = input("Enter your choice (1-5): ").strip()

        if choice == '1':
            print("\n[+] Encrypt module: Coming Soon!")
        elif choice == '2':
            print("\n[+] Decrypt module: Coming Soon!")
        elif choice == '3':
            print("\n[+] Attack module: Coming Soon!")
        elif choice == '4':
            print("\n[+] Analyze module: Coming Soon!")
        elif choice == '5':
            print("\nExiting CryptoLabX. Goodbye!")
            sys.exit(0)
        else:
            print("\n[!] Invalid choice! Please select a valid option (1-5).")

if __name__ == "__main__":
    main()
