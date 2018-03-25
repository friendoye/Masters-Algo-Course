
if __name__ == "__main__":
    for i in range(1, 50):
        if i % 2 == 1:
            continue
        if i % 15 == 0:
            print "FizzBuzz"
        elif i % 3 == 0:
            print "Fizz"
        elif i % 5 == 0:
            print "Bazz"
        else:
            print str(i)
