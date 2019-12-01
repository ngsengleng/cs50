from cs50 import get_int

def main():
    while True:
        height = get_int("Height: ")
        if height > 0 and height <= 8:
            break
    pyramid(height)

def pyramid(height):
    for i in range(height):
        for j in range(height - i - 1):
            print(" ", end="")
        for k in range(i + 1):
            print("#",end="")
        print("  ",end="")
        for l in range(i + 1):
            print("#",end="")
        print()

if __name__ == "__main__":
    main()