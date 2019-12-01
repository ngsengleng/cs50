from cs50 import get_string
from sys import argv

if len(argv) != 2:
    print("Usage: python caesar.py k")
    exit(1)
    
key = int(argv[1],10) % 26
input = get_string("plaintext: ")
print("ciphertext: ", end="")

for char in input:
    if char.isalpha():
        new = ord(char) + key
        if (new > 90 and new < 97) or (new > 90 and new > 122):
            new = new - 26
        newer = chr(new)
    else:
        newer = char

    
    print(f"{newer}", end="")
print("")