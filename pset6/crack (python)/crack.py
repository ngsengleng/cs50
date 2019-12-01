from sys import argv
from crypt import crypt

#user input
if len(argv) != 2:
    print("Usage: python crack.py hash")
    exit(1)

#retrieving salt
salt = argv[1][:2]

char = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
list1 = [" "," "," "," "," "]
m = 0
n = 0
pos = 0
word = ""
length = 1
count = 0
#generating potential password and hash
for i in range(53):
    for j in range(53):
        for k in range(53):
            for l in range(53):
                for m in range(53):
                    count += 1
                    list1[0] =  char[m]                    
                    list1[1] =  char[l]
                    list1[2] =  char[k]
                    list1[3] =  char[j]
                    list1[4] =  char[i]
                    word = ''.join(list1)
                    word = word.replace(" ", "")
                    len1 = len(word)
                    if length > len1:
                        continue
                 #   print(word)
                    hashd = crypt(word, salt)
                    if hashd == argv[1]:
                        print(word)
                        exit()
                if length < 2:
                    length = 2
            if length < 3:
                length = 3
        if length < 4:
            length = 4
    if length < 5:
        length = 5