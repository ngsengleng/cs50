from sys import argv
from cs50 import get_string

if len(argv) != 2 or argv[1].isalpha() == False:
    print("Usage: python vigenere.py k")
    exit(1)

#convert key into more workable format
key = argv[1].lower()

uInput = get_string("plaintext: ")
print("ciphertext: ", end = "")
#define variables

i = 0
list1 = []

#cipher
for char in uInput:
    change = char
    if char.isalpha():
        shift = ord(key[i]) - ord('a')
        change = ord(char) + shift
        if char.isupper() and change > ord('Z'):
            change = change - 26
        if char.islower() and change > ord('z'):
            change = change - 26
        change = chr(change)
        i += 1
        if i == len(argv[1]):
            i = 0
    list1.extend(change)
list1 = ''.join(list1)
print(list1)
