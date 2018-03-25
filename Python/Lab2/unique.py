
def get_unique(numbers):
    s = set(numbers)
    return [(x, numbers.count(x)) for x in s]

if __name__ == "__main__":
    target_numbers = [1,5,2,3,5,6,8,3,5,8,4,6,9,3,5,8,5]
    print get_unique(target_numbers)
    print get_unique(target_numbers)
    print sorted(target_numbers)
