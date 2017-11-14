# get input from user
while 1:
    height = int(input("Height: "))
    if height >= 0 and height <= 23:
        break
# print the pyramid
for i in range(1, height + 1):
    # the first half of the pyramid
    for k in range(0, height - i):
        print(" ", end="")
    for k in range(0, i):
        print("#", end="")
    # middle of the pyramid
    for k in range(0, 2):
        print(" ", end="")
    # the second half of pyramid
    for k in range(0, i):
        print("#", end="")
    # new level in pyramid
    print("")
