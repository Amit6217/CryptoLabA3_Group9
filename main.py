import sys
from analysis.analyzer import analyze_file
from utils.logger import log_activity
from utils.menu import display_menu


def main():
    while True:
        display_menu()
        choice = input("Enter your choice (1-5): ").strip()
        if choice == '1':
            log_activity("Encrypt")
            print("\n[+] Encrypt module: Coming Soon!")
        elif choice == '2':
            log_activity("Decrypt")
            print("\n[+] Decrypt module: Coming Soon!")
        elif choice == '3':
            log_activity("Attack")
            print("\n[+] Attack module: Coming Soon!")
        elif choice == '4':
           log_activity("Analyze")
           analyze_file()
        elif choice == '5':
            log_activity("Exit")
            print("\nExiting CryptoLabA3_Group9. Goodbye!")
            sys.exit(0)
        else:
            print("\n[!] Invalid choice! Please select a valid option (1-5).")

if __name__ == "__main__":
    main()
