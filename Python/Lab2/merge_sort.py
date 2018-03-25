
def sort(objects):
    if len(objects) == 1:
        return objects
    else:
        mid = len(objects) / 2
        left_part = sort(objects[:mid])
        right_part = sort(objects[mid:])

        l_index = 0
        r_index = 0
        buffer = objects[mid:mid]

        while l_index < len(left_part) and r_index < len(right_part):
            if left_part[l_index] < right_part[r_index]:
                buffer = buffer + left_part[l_index:l_index + 1]
                l_index = l_index + 1
            else:
                buffer = buffer + right_part[r_index:r_index + 1]
                r_index = r_index + 1
        buffer = buffer + right_part[r_index:] + left_part[l_index:]

        return buffer

if __name__ == "__main__":
    #target_numbers = [1, 5, 2, 3, 5, 6, 8, 3, 5, 8, 4, 6, 9, 3, 5, 8, 5]
    target_numbers = [2]
    print target_numbers
    sorted()
    print sort(target_numbers)