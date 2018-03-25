
def is_lucky_ticket(ticket_number):
    if test_is_lucky(ticket_number):
        return ticket_number
    else:
        upper_number = ticket_number
        lower_number = ticket_number
        while True:
            upper_number += 1
            lower_number -= 1
            if upper_number == 1000000:
                upper_number = 0
            if lower_number == -1:
                lower_number = 999999
            if test_is_lucky(upper_number):
                return upper_number
            if test_is_lucky(lower_number):
                return lower_number

def test_is_lucky(ticket_number):
    first_sum = numbers_sum(ticket_number / 1000)
    second_sum = numbers_sum(ticket_number % 1000)
    return first_sum == second_sum

def numbers_sum(number):
    sum = 0
    while (number != 0):
        part = number % 10
        sum += part
        number /= 10
    return sum


if __name__ == "__main__":
    ticket_number = 10
    print is_lucky_ticket(ticket_number)