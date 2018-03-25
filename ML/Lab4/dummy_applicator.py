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

model = sys.argv[-2]
fname = sys.argv[-1]

with open(model, 'rt') as fout:
    threshold = int(fout.read())

if np.sum(read_file(fname)) <= threshold:
    print 0
else:
    print 1
