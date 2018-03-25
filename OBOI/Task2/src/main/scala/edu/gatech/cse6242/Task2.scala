package edu.gatech.cse6242

import org.apache.hadoop.fs.FileSystem
import org.apache.hadoop.fs.Path
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.SparkConf

object Task2 {
  def main(args: Array[String]) {
    val sc = new SparkContext(new SparkConf().setAppName("Task2"))

    // read the file
    val file = sc.textFile("hdfs://localhost:8020" + args(0))

    val partialVertexWeightsPairs = file
      .map(_.split(" +"))
      .map(tokens => Tuple2(tokens(1).toInt, tokens(2).toLong))
      //(_(1).toInt, _[2].toLong)))

    val fullVertexWeightsPairs = partialVertexWeightsPairs.reduceByKey(_ + _)

    // store output on given HDFS path.
    val result = fullVertexWeightsPairs.collect()
      .map(pair => s"${pair._1} ${pair._2}")
      .mkString("\n")

    writeToFile(sc, "hdfs://localhost:8020" + args(1), result)
    // YOU NEED TO CHANGE THIS
    // file.saveAsTextFile("hdfs://localhost:8020" + args(1))
  }

  def writeToFile(sc: SparkContext, filename: String, result: String) {
    // Hadoop Config is accessible from SparkContext
    val fs = FileSystem.get(sc.hadoopConfiguration);

    // Output file can be created from file system.
    val output = fs.create(new Path(filename));

    // But BufferedOutputStream must be used to output an actual text file.
    val os = new BufferedOutputStream(output)

    os.write(result.getBytes("UTF-8"))
  }
}
