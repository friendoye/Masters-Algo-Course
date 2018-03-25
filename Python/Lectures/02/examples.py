
# coding: utf-8

# # Lecture 2 examples (Dzmitryi Kasitsyn)

# ### Simple import

# In[ ]:


import sys
sys.version


# In[ ]:


def is_fractions_imported():
    return 'fractions' in sys.modules

is_fractions_imported()


# In[ ]:


import fractions
is_fractions_imported()


# ### Import "future"

# In[ ]:


7 / 3


# In[ ]:


from __future__ import division
7 / 3


# ### Built-in functions

# In[ ]:


dir(__builtin__)  # just "dir()" in the Interpreter


# In[ ]:


number = 11
bin(number), oct(number), hex(number)


# In[ ]:


max(1, 3, 4, 2)  # collection/iterable may be also passed into


# In[ ]:


# infinite loop
while True:
    pass


# ### Lists

# In[ ]:


x = [3, 5, 9]
y = [3, 5, 9]

print("are objects equal: %s" % (x == y))

print("are objects same: %s (id of x - %d; id of y - %d)" % (x is y, id(x), id(y)))

print("x equals itself comparing values (%s) and identifiers (%s)" % (x == x, x is x))


# In[ ]:


x = []  # or x = list()
x.append(1)
x.insert(0, 2)  # insert 2 at index 0
x += [3, 4]
print(x)


# In[ ]:


x[0] == x[len(x) - 1] == x[-1]


# In[ ]:


x[0] = -1
x


# In[ ]:


x[100]


# In[ ]:


x.remove(2)
x.pop()
del x[0]
print(x)


# In[ ]:


x.index(3)


# In[ ]:


bool([])


# In[ ]:


2 not in x


# In[ ]:


[1, 2, 3] > [1, 2, 1]


# In[ ]:


[1] * 5 == [1, 1, 1, 1, 1]


# In[ ]:


reversed([1, 3, 7])  # try list(reversed(...))


# ### Tuples

# In[ ]:


x = (1, 2, 3)  # or just x = 1, 2, 3
y = tuple([5, 6, 7])


# In[ ]:


x[0] = 2


# In[ ]:


x += 1, 2
x


# In[ ]:


sorted(x)


# ### Iterating over multiple collections

# In[ ]:


for x, y in zip([1, 2, 3], [-3, -2, -1]):  # not the same as two nested "for" cycles!
    print(x % y == 0)


# In[ ]:


for idx, item in enumerate([1, 3, 5]):
    print('item at position %d equals %d' % (idx, item))


# ### Slices

# In[ ]:


x = list(range(10))
print(x[0:10:2])


# In[ ]:


x[100:110]


# In[ ]:


x[0:10:-2]


# In[ ]:


print(x[9:0:-2])
print(x[-1:0:-2])
print(x[::-2])


# In[ ]:


# x:y:z is an object named "slice"
print(x[slice(9, 0, -2)])
print(x[slice(-1, 0, -2)])


# In[ ]:


even_indices_slice = slice(None, None, 2)
print x[even_indices_slice]


# ### Nested lists and comprehensions

# In[ ]:


x = [[], [], []]
y = [[]] * 3

z = []
for _ in range(3):
    z.append([])
    
print(x, y, z)


# In[ ]:


x[0].append(1)
y[0].append(2)
z[0].append(3)

print(x, y, z)


# In[ ]:


z_pretty = [[] for _ in range(3)]
z_pretty[0].append(3)
print(z_pretty)


# ### Functional programming

# In[ ]:


def is_odd(x):
    return bool(x % 2)


# In[ ]:


map(is_odd, range(10)) == [is_odd(x) for x in range(10)]  # see Py3 note


# In[ ]:


filter(is_odd, range(10)) == [x for x in range(10) if is_odd(x)]  # see Py3 note

# try None istead of is_odd


# In[ ]:


any(map(is_odd, range(10)))

# try an empty list


# In[ ]:


all(map(is_odd, range(1, 10, 2)))

# try an empty list


# In[ ]:


another_comprehension = (is_odd(x) for x in range(10))
are_equal_sequences = (list(another_comprehension) == [is_odd(x) for x in range(10)])

# note about Python 3.x
print(another_comprehension, are_equal_sequences)


# ### Dictionaries

# In[ ]:


x = {}  # or x = dict()


# In[ ]:


hash(1)


# In[ ]:


hash(True)


# In[ ]:


hash((1, 2, 3))


# In[ ]:


hash(tuple())


# In[ ]:


hash([1, 2, 3])


# In[ ]:


hash(x)


# In[ ]:


x = dict(zip(range(3), range(6, 9)))
y = {2: 8, 1: 7, 0: 6}
print(x, len(x))


# In[ ]:


x[0] == x.get(0)


# In[ ]:


print(100500 in x)
print(x.get(100500, None))


# In[ ]:


x[100500] = 0
x.setdefault(0, -1)
x.setdefault(None, None)  # try update method too
x


# In[ ]:


del x[None]  # could we write just "del x[]"?

y = x.pop(100500)  # try popitem and clear methods
print(y, x)


# In[ ]:


x.items()


# In[ ]:


x == y  # refer to items() method


# In[ ]:


0 in x.iterkeys() and 8 in x.viewvalues()  # see Py3 note


# In[ ]:


w = x.viewitems()
i = x.iteritems()

x[3] = 9


# In[ ]:


for k, v in i:
    print(k, v)


# In[ ]:


for k, v in w:
    print(k, v)


# In[ ]:


dict(zip(range(3), range(6, 9))) == {k: v for k, v in zip(range(3), range(6, 9))}


# ### Set and frozenset

# In[ ]:


x = set()
x_frozen = frozenset()


# In[ ]:


hash(x)


# In[ ]:


hash(x_frozen)


# In[ ]:


x.add(1)
x.add(2)
x.add(1)
x


# In[ ]:


x.remove(2)
x.discard(2)

# try pop and clear


# In[ ]:


# all logical operations are available: & (intersect), | (union), ^ (symmetric_difference), - (difference)
x = {1, 2, 3}
x &= {1, 2, 6}  # intersect_update
x


# In[ ]:


x.update({2, 4, 5})  # "x |= ..." or "x = x | ..." or "x = x.union(...)"
x


# In[ ]:


x - {3, 4}

