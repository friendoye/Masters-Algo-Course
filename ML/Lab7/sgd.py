#!/usr/bin/env python2
import os.path
import sys
import random
import numpy as np
from math import exp

def file_line_to_feature_with_label(line):
	literals = line.split(",")
	return [np.array([float(x) for x in literals[:-1]]), literals[-1]]

labels = ["Iris-setosa", "Iris-versicolor", "Iris-virginica"]
T = 50
delta = 0.1
lmbd = 0.1
k = 10

TURN_BATCH_ON = False

def calc_logistical_grad_i(wx, y):
	(w, x) = wx
	ywx = y * w * x
	if ywx >= 500: 
		return 2 * lmbd * w
	else:
		return 2 * lmbd * w - y * x / (1.0 + exp(y * w * x))
	

def calc_subgradient(w, x, y):
	result = map(lambda wx: calc_logistical_grad_i(wx, y), zip(w, x))
	return np.array(result)

def logistic_func(x):
	if x >= 500: 
		return 1.0
	else:
		return 2 * exp(x) / (1.0 + exp(x)) - 1

def calc_risk(w_Ls, features, label):
	if (len(features) == 0):
		raise "Zero features for calc_risk!"
	summa = 0.0
	for f in features:
		result_of_classifier = logistic_func(sum(w_Ls * f[0])) >= 0.0
		true_result = label == f[1]
		summa += (result_of_classifier != true_result)
	return summa / len(features)

def kFold(features, k, label):
	np.random.shuffle(features)
	splits = np.array_split(features, k)
	train_risk = []
	validation_risk = []
	for t in range(k):
		validation_split = splits[t]
		train_split = np.concatenate(np.delete(splits, t))
		#print validation_split
		#print train_split
		#train_split = np.concatenate(np.copy(train_split))
		w_Ls = sgd(train_split, label)
		#print "W for Ls: %s" % w_Ls
		train_risk.append(calc_risk(w_Ls, train_split, label))
		validation_risk.append(calc_risk(w_Ls, validation_split, label))
	avg_train_risk = sum(train_risk) / k
	avg_validation_risk = sum(validation_risk) / k
	#print "Train result for %s : %s" % (label, avg_train_risk)
	#print "Validation result for %s : %s" % (label, avg_validation_risk)
	return avg_validation_risk

def sgd(features, label):
	w_array = [np.zeros(len(features[0][0]))]
	for t in range(1, T + 1):
		feature = random.choice(features)
		
		subgradient = 0.0
		y_error = 0.0
		if not TURN_BATCH_ON:
			y = 2 * (feature[-1] == label) - 1 # {-1, 1}
			y_error = abs(y - logistic_func(sum(w_array[t-1] * feature[0]))) / 2
			subgradient = calc_subgradient(w_array[t - 1], feature[0], y)
		else:
			for f in features:
				y = 2 * (f[-1] == label) - 1 # {-1, 1}
				y_error += abs(y - logistic_func(sum(w_array[t-1] * f[0]))) / 2
				subgradient += calc_subgradient(w_array[t - 1], f[0], y)
			subgradient /= len(features)
			y_error /= len(features)
		
		#print "(%s)#%s error: %s" % (label, t, y_error)
		w_array.append(w_array[t - 1] - delta * subgradient)
	return sum(w_array) / T


if __name__ == "__main__":
	train_file_path = sys.argv[-1]
	features = []
	with open(train_file_path) as train_file:
		features = [file_line_to_feature_with_label(s.strip()) for s in train_file.readlines()]
		#for i in range(len(features)):
	#		features[i][0] = np.insert(features[i][0], 0, 1.0)
	#print(features)

	min_args = [-1, 1.0]
	for T0 in np.arange(50, 1000, 50):
		for delta0 in np.arange(0.1, 2.0, 0.1):
			for lmbd0 in np.arange(0.05, 1.0, 0.05):
				T = T0
				delta = delta0
				lmbd = lmbd0
				error = kFold(features, k, labels[0])
				if error < min_args[1]:
					min_args[0] = (T, delta, lmbd)
					min_args[1] = error
	print min_args
	#kFold(features, k, labels[1])
	#kFold(features, k, labels[2])
	# setosa_w     = sgd(features, labels[0])
	# versicolor_w = sgd(features, labels[1])
	# virginica_w  = sgd(features, labels[2])