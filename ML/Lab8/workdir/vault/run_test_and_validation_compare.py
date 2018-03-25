import os
import sys
import shutil
from subprocess import call

def create_workdir(scaled_train_path, workdir_name):
	parent_train_dir = os.path.dirname(scaled_train_path)
	workdir_path = os.path.join(parent_train_dir, workdir_name)
	if os.path.exists(workdir_path):
		shutil.rmtree(workdir_path)
	os.makedirs(workdir_path)
	return workdir_path

def split_train_into(scaled_train_path, workdir_name):
	parent_train_dir = os.path.dirname(scaled_train_path)
	workdir_path = os.path.join(parent_data_dir, workdir_name)
	if os.path.exists(workdir_path):
		shutil.rmtree(workdir_path)
	os.makedirs(workdir_path)
	return workdir_path

def write_lines_to_file(file_path, lines):
	with open(file_path, "w") as temp_file:
		temp_file.writelines(lines)

def call_libsvm_train(d, C_k, input_dataset_name, model_name):
	terminal_command = "svm-train -s 0 -t 1 -q " + \
		("-d %s " % d) + \
		("-c %s " % 2 ** C_k) + \
		("%s %s" % (input_dataset_name, model_name))
	#print terminal_command
	call([terminal_command], shell=True)

def call_libsvm_predict(input_dataset_name, model_name, predict_result_name, 
						stdout_file_name):
	terminal_command = "svm-predict %s %s %s" % \
		(input_dataset_name, model_name, predict_result_name)
	#print terminal_command + (" > %s" % stdout_file_name)
	with open(stdout_file_name, "w") as output_pipe:
		call([terminal_command], stdout=output_pipe, shell=True)
	numbers = None
	with open(stdout_file_name, "r") as output_pipe:
		result_prompt = output_pipe.readline().strip()
		numbers = result_prompt.split(" ")[3][1:-1].split("/")
	return float(numbers[0]) / float(numbers[1])

def kFold(workdir, k, d, C_k, features, output):
	train_file_path = os.path.join(workdir, "k_fold_train")
	validation_file_path = os.path.join(workdir, "k_fold_validation")
	svm_model_path = os.path.join(workdir, "svm_model")
	validation_predict_path = os.path.join(workdir, "validation_predict")
	validation_result_path = os.path.join(workdir, "validation_result")
	
	avg_chunk_size = len(features) // k
	avg_valid_result = 0.0
	for i in range(k):
		validation_slice = features[i * avg_chunk_size : (i + 1) * avg_chunk_size]
		train_slice = features[:i * avg_chunk_size] + \
					  features[(i + 1) * avg_chunk_size:]
		write_lines_to_file(validation_file_path, validation_slice)
		write_lines_to_file(train_file_path, train_slice)
		call_libsvm_train(d, C_k, train_file_path, svm_model_path)
		validation_result = call_libsvm_predict(validation_file_path, svm_model_path, \
			validation_predict_path, validation_result_path)
		#report_string = "Validation for (d=%s, C=%s, fold_num=%s): %s" % (d, 2 ** C_k, i + 1, validation_result * 100)
		#print report_string
		#output.write(report_string + "\n")

		avg_valid_result += validation_result
	avg_valid_result /= k
	report_string = "Validation for (d=%s, C=%s): %s" % (d, 2 ** C_k, avg_valid_result)
	print report_string
	output.write(report_string + "\n")

def test_set_check(workdir, d_list, C_k, scaled_train_path, 
	               scaled_test_path):
	test_predict_path = os.path.join(workdir, "test_predict")
	test_result_path = os.path.join(workdir, "test_result")
	svm_model_path = os.path.join(workdir, "svm_model_for_test")
	test_on_diff_d_path = os.path.join(workdir, "test_results_on_diff_d")
	log_path = os.path.join(workdir, "sv_info")

	with open(test_on_diff_d_path, "w") as output, open(log_path, "w") as log_output:
		for d in d_list:
			call_libsvm_train(d, C_k, scaled_train_path, svm_model_path)
			result = call_libsvm_predict(scaled_test_path, svm_model_path, \
				test_predict_path, test_result_path)
			log_sv_amount(svm_model_path, d, C_k, log_output)
			output.write("Test result for (d=%s, C=%s): %s\n" % (d, 2 ** C_k, result))

def log_sv_amount(model_path, d, C_k, log_output):
	with open(model_path) as model_input:
		lines = model_input.readlines()
		total_SV = lines[6].split(" ")[1].strip()
		partion_of_SV = lines[9][6:].strip()
		SV_info = "%s %s" % (total_SV, partion_of_SV)
		log_output.write("SV amount for (d=%s, C=%s): %s\n" % (d, 2 ** C_k, SV_info))

if __name__ == "__main__":
	scaled_train_path = sys.argv[-4]
	scaled_test_path = sys.argv[-3]
	C_k = int(sys.argv[-2])
	d = list(range(1, int(sys.argv[-1]) + 1))

	stat_workdir = create_workdir(scaled_train_path, "for_test_and_valid")
	report_file_path = os.path.join(stat_workdir, "train_report")
	report_file_path = os.path.join(stat_workdir, "valid_report")
	
	test_set_check(stat_workdir, d, C_k, \
		scaled_train_path, scaled_test_path)

	train_features = []
	with open(scaled_train_path) as train_file:
		train_features = train_file.readlines()
	
	with open(report_file_path, "w") as report_file:
		for curr_d in d:
			kFold(stat_workdir, 10, curr_d, C_k, train_features, report_file)