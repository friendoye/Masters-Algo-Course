#!/usr/bin/env python2

import os.path
import sys
import random
import shutil

if __name__ == "__main__":
	data_source = sys.argv[-3]
	parent_data_dir = os.path.dirname(data_source)
	train_and_validation_part = int(sys.argv[-2])
	test_part = int(sys.argv[-1])
	part_summa = train_and_validation_part + test_part

	with open(data_source) as data_file:
		data = data_file.readlines()
		random.shuffle(data)
		data = list(filter(None, data))
		data = [x for x in data if x.strip()]
		splitting_index = int(train_and_validation_part * len(data) / part_summa)
		print(splitting_index)
		if os.path.exists(os.path.join(parent_data_dir, "data")):
			shutil.rmtree(os.path.join(parent_data_dir, "data"))
		os.makedirs(os.path.join(parent_data_dir, "data"))
		with open(os.path.join(parent_data_dir, "data/train.data"), "w+") as train_file:
			train_file.writelines(data[:splitting_index])
		with open(os.path.join(parent_data_dir, "data/test.data"), "w+") as test_file:
			test_file.writelines(data[splitting_index:])