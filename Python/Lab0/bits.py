
def set_bit(number, position):
    return number | (1 << position)

def clear_bit(number, position):
    return number & ~(1 << position)

def test_bit(number, position):
    return number & (1 << position) != 0


if __name__ == "__main__":
    test_number = 0b11100100
    print bin(test_number)
    print bin(set_bit  (test_number, 4))
    print bin(clear_bit(test_number, 4))
    print bin(set_bit  (test_number, 5))
    print bin(clear_bit(test_number, 5))
    print test_bit(test_number, 4)
    print test_bit(test_number, 7)