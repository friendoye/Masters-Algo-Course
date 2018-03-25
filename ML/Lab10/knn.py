#!/usr/bin/env python2
import os.path
import sys
import random
import numpy as np
from collections import Counter

def file_line_to_feature_with_label(line):
	literals = line.split(",")
	return [np.array([float(x) for x in literals[:-1]]), literals[-1]]

def calc_risk(classifier, features):
	if (len(features) == 0):
		raise "Zero features for calc_risk!"
	summa = 0.0
	for f in features:
		summa += (classifier.predict(f[0]) != f[1])
	return summa / len(features)

def kFold(classifier, features, k):
	np.random.shuffle(features)
	splits = np.array_split(features, k)
	train_risk = []
	validation_risk = []
	for t in range(k):
		validation_split = splits[t]
		train_split = np.concatenate(np.delete(splits, t))
		classifier.train(train_split)
		validation_risk.append(calc_risk(classifier, validation_split))
	avg_train_risk = sum(train_risk) / k
	avg_validation_risk = sum(validation_risk) / k
	return avg_validation_risk

def eucl_distance(x, y):
	return sum([(x0 - y0) ** 2 for (x0, y0) in zip(x, y)]) ** 0.5

class KnnClassifier:

	def __init__(self, k, normalize=False):
		self.train_model = []
		self.k = k
		self.normalize = normalize

	def train(self, features):
		self.train_model = np.array(features.copy())
		if self.normalize:
			arr = np.array(list(self.train_model[:, 0]))
			self.min_features_bound = arr.min(0)
			self.features_ptp = arr.ptp(0)
			self.train_model[:, 0] = list((arr - arr.min(0)) / arr.ptp(0))

	def predict(self, x):
		if self.normalize:
			x = (x - self.min_features_bound) / self.features_ptp
		nearest_features = sorted(self.train_model, key=lambda record: eucl_distance(record[0], x))
		counter = Counter([record[1] for record in nearest_features[:self.k]])
		#print "Predicting %s with %s" % (x, nearest_features[:self.k])
		#print "Result: %s" % counter.most_common(1)[0][0]
		return counter.most_common(1)[0][0]

if __name__ == "__main__":
	train_file_path = sys.argv[-2]
	test_file_path = sys.argv[-1]
	features = []
	test_features = []
	with open(train_file_path) as train_file:
		features = [file_line_to_feature_with_label(s.strip()) for s in train_file.readlines()]
	with open(test_file_path) as test_file:
		test_features = [file_line_to_feature_with_label(s.strip()) for s in test_file.readlines()]

	for k_for_knn in range(1, 21):
		classifier = KnnClassifier(k_for_knn, normalize=True)
		error = kFold(classifier, features, k_for_kfold)
		classifier.train(np.array(features))
		test_error = calc_risk(classifier, test_features)
		print "K-Fold Error for k-NN(%s): %s" % (k_for_knn, error)
		print "Test Error for k-NN(%s): %s" % (k_for_knn, test_error)