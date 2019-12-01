from cs50 import get_float

# user input
while True:
    change = get_float("Change owed: ")
    if change >= 0:
        break

#calculations
tup1 = (25, 10, 5, 1)
n = 0
coins = 0
change = change * 100
while True:
    while change >= tup1[n]:
        change = change - tup1[n]
        coins += 1
    n += 1
    if n == 4:
        break
print(f"{coins}")