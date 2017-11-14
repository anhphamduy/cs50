def check_valid(number, length):
    # total sum
    sum = 0

    # iterate through all the digits
    for i in range(1, length + 1):
        # get the current number
        num = number % 10
        # if the current number us every second digit
        if (i % 2 == 0):
            # double it and add it to the sum
            num *= 2
            while 1:
                sum += (num % 10)
                num //= 10
                if num == 0:
                    break
        else:
            # add normal digit to the sum
            sum += num
        # remove the last digit
        number //= 10

    # return true if divisible by 10 otherwise false
    if sum % 10 == 0:
        return True
    return False

# get card number
card_number = input("Number: ")

# check to see if the card's valid
if (check_valid(int(card_number), len(card_number))):
    # if it is valid and then check its type
    if (card_number[0] == "4" and (len(card_number) == 13 or
        len(card_number) == 16)):
        print("VISA")
    elif (card_number[:2] == "51" or card_number[:2] == "52" or
            card_number[:2] == "53" or card_number[:2] == "54" or
            card_number[:2] == "55") and len(card_number) == 16:
        print("MASTERCARD")
    elif (card_number[:2] == "34" or card_number[:2] == "37") and len(card_number) == 15:
        print("AMEX")

    else:
        print("INVALID")
else:
    print("INVALID")
