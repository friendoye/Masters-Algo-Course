
def calculate_special_sum(n):
    return sum([x * (x-1) for x in xrange(1, n + 1)])

if __name__ == "__main__":
    print calculate_special_sum(2)