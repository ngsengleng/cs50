from cs50 import get_string
from sys import argv


def main():
    if len(argv) != 2:
        print("python bleep.py dictionary")
        exit(1)
        
    #user input
    print("What message would you like to censor?")
    line = get_string()
    inpt = line.split()
    outpt = []
    
    #creating checklist
    banned = open(argv[1], 'r')
    listed = banned.readlines()
    
    #removing the \n in checklist
    for i in range(len(listed)):
        listed[i] = listed[i].strip('\n')
    
    #call check function
    printers(listed, inpt, outpt)
    
def printers(listed, inpt, outpt):
    
    muted = "***********************************************"
    length = len(inpt)
    
    #checking for matches
    for i in range(length):
        word = inpt[i].lower()
        if word in listed:
            new = muted[:len(inpt[i])]
        else:
            new = inpt[i]
        outpt.append(new)
        if i < length:
            outpt.append(' ')
    out = ''.join(outpt)
    print(out)
    
if __name__ == "__main__":
    main()

