#!/usr/bin/env python2

import os.path
import sys
import random
import shutil

def svm_labeled_format(line):
	values = line.split(",")
	result = values[-1] + " "
	for i in xrange(len(values) - 1):
		result += "%s:%s " % (i + 1, values[i])
	return result.strip() + "\n"

def write_to(file_name, lines):
	with open(file_name, "w+") as f:
		f.writelines(lines)

if __name__ == "__main__":
	data_path = sys.argv[-4]
	svm_labeled_path = sys.argv[-3]
	train_samples_amount = int(sys.argv[-2])
	test_svm_labeled_path = sys.argv[-1]
	
	with open(data_path) as data_file:
		data = data_file.readlines()
		data = list(filter(None, data))
		print data
		svm_labeled = [svm_labeled_format(x.strip()) for x in data if x.strip()]
		
		if (train_samples_amount == None):
			train_samples_amount = len(svm_labeled)

		write_to(svm_labeled_path, svm_labeled[:train_samples_amount])
		if (test_svm_labeled_path != None):
			write_to(test_svm_labeled_path, svm_labeled[train_samples_amount:])