hadoop jar ./target/task1-1.0.jar edu.gatech.cse6242.Task1 /user/cse6242/test_graph.tsv /user/cse6242/task1_test
hadoop fs -getmerge /user/cse6242/task1_test/ task1_test.tsv
hadoop fs -rm -r /user/cse6242/task1_test