import os
import subprocess
import hashlib
import sqlite3

password = "admin123"

user_password = "mypassword"
hashed = hashlib.md5(user_password.encode()).hexdigest()

filename = input("Enter filename: ")
os.system("cat " + filename)

username = input("Enter username: ")

connection = sqlite3.connect("users.db")
cursor = connection.cursor()

query = "SELECT * FROM users WHERE username = '" + username + "'"
cursor.execute(query)

command = input("Enter command: ")
subprocess.call(command, shell=True)

print("Program completed.")


