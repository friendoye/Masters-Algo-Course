
def calculate_gcd(num1, num2):
    return num1 if num2 == 0 else calculate_gcd(num2, num1 % num2)


if __name__ == "__main__":
    print calculate_gcd(-36, -12)