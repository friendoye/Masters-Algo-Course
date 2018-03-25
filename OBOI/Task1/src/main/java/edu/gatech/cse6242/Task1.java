package edu.gatech.cse6242;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.*;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.JobClient;
import org.apache.hadoop.mapreduce.*;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.util.*;

import java.io.IOException;
import java.util.StringTokenizer;

public class Task1 extends Configured implements Tool {

  public static class Map extends Mapper<LongWritable, Text, IntWritable, LongWritable> {
    private IntWritable vertexNum = new IntWritable();
    private LongWritable incomingWeight = new LongWritable();

    @Override
    public void map(LongWritable key, Text value, Context context)
            throws IOException, InterruptedException {
      String line = value.toString();
      StringTokenizer tokenizer = new StringTokenizer(line);
      while (tokenizer.hasMoreTokens()) {
        tokenizer.nextToken();
        IntWritable vertexNum = new IntWritable(Integer.valueOf(tokenizer.nextToken()));
        LongWritable incomingWeight = new LongWritable(Integer.valueOf(tokenizer.nextToken()));
        context.write(vertexNum, incomingWeight);
      }
    }
  }

  public static class Reduce extends Reducer<IntWritable, LongWritable, IntWritable, LongWritable> {
    @Override
    protected void reduce(IntWritable key, Iterable<LongWritable> values, Context context)
            throws IOException, InterruptedException {
      long sum = 0;
      for (LongWritable val : values) {
        sum += val.get();
      }
      context.write(key, new LongWritable(sum));
    }
  }

  @Override
  public int run(String[] args) throws Exception {
    Configuration conf = new Configuration();
    Job job = new Job(conf, "Task1");
    job.setJarByClass(getClass());

    job.setOutputKeyClass(IntWritable.class);
    job.setOutputValueClass(LongWritable.class);

    job.setMapperClass(Map.class);
    job.setReducerClass(Reduce.class);

    job.setMapOutputKeyClass(IntWritable.class);
    job.setMapOutputValueClass(LongWritable.class);

    job.setInputFormatClass(TextInputFormat.class);
    job.setOutputFormatClass(TextOutputFormat.class);

    FileInputFormat.addInputPath(job, new Path(args[0]));
    FileOutputFormat.setOutputPath(job, new Path(args[1]));

    job.waitForCompletion(true);
    return 0;
  }

  public static void main(String[] args) throws Exception {
    int res = ToolRunner.run(new Configuration(), new Task1(), args);
    System.exit(res);
  }
}
