package fr.claisse;

import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.Counter;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;

public class Summary {
    public static class SummaryMapper extends Mapper<Object, Text, IntWritable, IntWritable> {
        // These counters will be shown at the end of the job, together with other mapreduce counters (this was not asked for)
        enum CountersEnum {
            NB_CITIES,
            NB_POP,
            TOTAL_POP
        }

        private int prec = 10;

        protected void setup(Mapper.Context context) throws IOException, InterruptedException {
            prec = (int) Long.parseLong(context.getConfiguration().get("precision"));
        }

        public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
            //the mapper will send out (to the reducer) pairs (log population, population)
            IntWritable one = new IntWritable(1);
            IntWritable out_key;
            String [] parsed = value.toString().split(",");
            String pop = parsed[4];

            if (!parsed[0].equals("Country")) { // filter out the first line
                if (!pop.isEmpty()) { // filter out cities without population information
                    int this_pop = Integer.parseInt(pop);
                    context.write(new IntWritable((int) (Math.log10(this_pop)/Math.log10(prec))), new IntWritable(this_pop)); // output is log(pop), 1
                    // now update counters
                    Counter city_pop_counter = context.getCounter(CountersEnum.class.getName(), CountersEnum.NB_POP.toString());
                    city_pop_counter.increment(1);
                    Counter pop_counter = context.getCounter(CountersEnum.class.getName(), CountersEnum.TOTAL_POP.toString());
                    pop_counter.increment(this_pop);
                }
                //one more city
                Counter city_counter = context.getCounter(CountersEnum.class.getName(), CountersEnum.NB_CITIES.toString());
                city_counter.increment(1);
            }
        }
    }

    public static class SummaryReducer extends Reducer<IntWritable, IntWritable, IntWritable, Text> {
        private int prec = 10;

        protected void setup(Context context) throws IOException, InterruptedException {
            prec = (int) Long.parseLong(context.getConfiguration().get("precision"));
            context.write(new IntWritable(0), new Text("count\tavg\tmax\tmin"));
        }

        public void reduce(IntWritable key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException {
            // the reducer will receive all populations for a given (log population), calculate count, min, max, mean, and then make a string with that information to write to the output file
            int sum = 0;
            int count = 0;
            int max = 0;
            int min = -1;
            int this_value;
            for (IntWritable value : values) {
                this_value = value.get();
                sum += this_value;
                count++;
                if ((min == -1) || (min > this_value) )
                    min = this_value;
                if (max < this_value)
                    max = this_value;
            }
            String value = count + "\t" + (count > 0 ? sum / count : 0) + "\t" + max + "\t" + min;
            context.write(new IntWritable((int) Math.pow(prec,key.get())), new Text(value));
        }
    }

    public static class MonJob extends Configured implements Tool {
        public int run(String[] args) throws Exception {
            Configuration conf = getConf();
            // here we could make the precision adjustable by parameter (for instance, instead of using log10, using log2). That was not asked for in the exercise.
            conf.set("precision", "10");
            Job job = Job.getInstance(conf, "Summary_MapReduce");

            job.setNumReduceTasks(1);
            job.setJarByClass(Summary.class);
            job.setMapperClass(SummaryMapper.class);
            job.setMapOutputKeyClass(IntWritable.class);
            job.setMapOutputValueClass(IntWritable.class);
            job.setReducerClass(SummaryReducer.class);
            job.setOutputKeyClass(IntWritable.class);
            job.setOutputValueClass(Text.class);
            job.setOutputFormatClass(TextOutputFormat.class);
            job.setInputFormatClass(TextInputFormat.class);
            FileInputFormat.addInputPath(job, new Path(args[0]));
            FileOutputFormat.setOutputPath(job, new Path(args[1]));

            return job.waitForCompletion(true) ? 0 : 1;
        }
    }

    public static void main(String[] args) throws Exception {
        int res = ToolRunner.run(new Configuration(), new MonJob(), args);
        System.exit(res);
    }
}
