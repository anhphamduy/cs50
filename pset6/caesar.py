import sys

if len(sys.argv) != 2:
    print("Usage: python caesar.py key")
    exit(1)
key = int(sys.argv[1])
plaintext = input("plaintext: ")
ciphertext = ""
for i in plaintext:
    if i.isalpha():
        if i.islower():
            ciphertext += chr((ord(i) - 97 + key) % 26 + 97)
        else:
            ciphertext += chr((ord(i) - 65 + key) % 26 + 65)
    else:
        ciphertext += i

print("ciphertext:", ciphertext)

