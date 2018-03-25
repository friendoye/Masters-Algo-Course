
def get_sequence_item(iteration):
    if iteration == 0:
        return 0
    else:
        first_part = get_sequence_item(iteration - 1)
        second_part = (~first_part) & get_mask(iteration)
        return (first_part << (2 ** (iteration - 1))) + second_part

def get_mask(k):
    return 2 ** (2 ** (k - 1)) - 1


if __name__ == "__main__":
    k = 6
    # Hack. Adding "0" before print for better readability
    print get_sequence_item(6)
    print "0" + bin(get_sequence_item(k))[2:]