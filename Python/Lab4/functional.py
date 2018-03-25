from os import listdir
from os.path import isdir, join, dirname
from itertools import imap

def __stringToInt(text):
    if text[:2] == "0b":
        return int(text, 2)
    elif text[:2] == "0o":
        return int(text, 8)
    elif text[:2] == "0x":
        return int(text, 16)
    else:
        return int(text)

def __product_func(x, y):
    """
    _product_func multiplies number representation of x and y.
    """
    if not isinstance(x, (int, long, float)):
        x = __stringToInt(x)
    if not isinstance(y, (int, long, float)):
        y = __stringToInt(y)
    return x * y

def scalar_product(iterable1, iterable2):
    try:
        return sum(imap(__product_func, iterable1, iterable2))
    except ValueError:
        return None

def __is_iterable(obj):
    """
    _is_iterable checks if given obj is iterable.
    """
    try:
        iterator = iter(obj)
    except TypeError:
        return False
    else:
        return True

def flatten(nested_iterable):
    stack = [iter(nested_iterable)]
    while len(stack) > 0:
        try:
            element = stack[-1].next()
            if __is_iterable(element) and not isinstance(element, str):
                stack.append(iter(element))
            else:
                yield element
        except StopIteration:
            stack.pop()

def walk_files(top_path):
    names = listdir(top_path)

    dirs = []
    files_and_dirs = []
    for name in names:
        if isdir(join(top_path, name)):
            dirs.append(name)
        else:
            files_and_dirs.append(name)

    if len(dirs) > 0:
        dirs_dict = {}
        for name in dirs:
            new_path = join(top_path, name)
            dirs_dict[name] = walk_files(new_path)

        files_and_dirs.append(dirs_dict)

    return files_and_dirs