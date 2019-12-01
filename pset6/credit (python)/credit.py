from cs50 import get_string
from sys import exit

tup1 = ('AMEX', 'MASTERCARD', 'VISA', 'INVALID')

#check for correctness of input
while True:
    input = get_string("Number: ")
    length = len(input)
    if input.isdigit():
            break

card = 3

# decide what company card it is
if input[0] == '4':
    if length == 16 or length == 13:
        card = 2
elif input[0] == '3':
    if input[1] == '4' or input[1] == '7' and length == 15:
        card = 0
elif input[0] == '5':
    if input[1] == '1' or input[1] == '2' or input[1] == '3' or input[1] == '4' or input[1] == '5':
        if length == 16:
            card = 1

#calculations
#define variables
n = 0
sum1 = 0
sum2 = 0
tSum = 0

for i in input:
    j = int(i)
    #16-digit numbers
    if n % 2 == 0 and length == 16:
        sum1 = sum1 + (j*2) // 10 + (j*2) % 10
    #13 and 15-digit numbers
    elif n % 2 == 1 and length != 16:
        sum1 = sum1 + (j*2) // 10 + (j*2) % 10
    else:
        sum2 = sum2 + j
    n += 1
#print(f"{i}_{j}_{n}_{sum1}")
tSum = sum1 + sum2
if tSum % 10 != 0:
    card = 3
    
print(f"{tup1[card]}")



