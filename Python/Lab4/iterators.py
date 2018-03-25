from itertools import izip

def unique(iterable):
    unique_items = set()
    for item in iterable:
        if item not in unique_items:
            unique_items.add(item)
            yield item

def transpose(iterables):
    return izip(*iterables)