#!/usr/bin/env python2
import numpy as np
import scipy
import scipy.misc
import sklearn
import sys
import os

def read_file(fname):
    image = scipy.misc.imread(fname)
    assert image.shape == (26, 40, 3)
    return image[:,:,0]

folder = sys.argv[-2]
model = sys.argv[-1]

examples = []
for fname in os.listdir(os.path.join(folder, 'cars')):
    feature = np.sum(read_file(os.path.join(folder, 'cars', fname)))
    examples.append([feature, 0])
for fname in os.listdir(os.path.join(folder, 'faces')):
    feature = np.sum(read_file(os.path.join(folder, 'faces', fname)))
    examples.append([feature, 1])

best = None
correct = 0
for threshold, _ in examples:
    result = 0
    for feature, cls in examples:
        if feature <= threshold:
            my = 0
        else:
            my = 1
        if my == cls:
            result += 1
    if correct < result:
        correct = result
        best = threshold

with open(model, 'wt') as fout:
    fout.write(str(result))
