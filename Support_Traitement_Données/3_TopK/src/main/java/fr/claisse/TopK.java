package bigdata;

import java.io.IOException;
import java.util.TreeMap;
import java.util.Map;
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
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;

public class TopK {
    public static class TopKMapper extends Mapper<Object, Text, IntWritable, Text>{
        /* The mapper keeps a treemap, which is updated in each call to the method map()
         * and always contains the top K cities seen so far. After all the calls to map(),
         * the cleanup method will send the top K of this mapper to the reducer.
         * The output of the map uses the population as the key and the list of cities with
         * that population as a value (this solution was designed to deal with a situation where
         * multiple cities have the same population)
         */
        private TreeMap<Integer,String> topK;
        private int K = 10;

        protected  void setup(Mapper.Context context) throws IOException, InterruptedException {
            K = (int) Long.parseLong(context.getConfiguration().get("K"));
            topK = new TreeMap<>();
        }

        public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
            String[] parsed = value.toString().split(",");
            String pop = parsed[4];

            if(!parsed[0].equals("Country")) { // filter out the first line
                if (!pop.isEmpty()) { // filter out cities without population information
                    int this_pop = Integer.parseInt(pop);
                    Integer this_key = this_pop;
                    // first we check if there is already another city with this population in the map
                    if (topK.containsKey(this_key)) { // we just add this city to the string
                        String old_value = topK.get(this_key);
                        topK.remove(this_key);
                        topK.put(this_key, old_value + ", " + parsed[1]);
                    } else {
                        // check if there are already K in our topK structure
                        if (topK.size() < K) // there aren't, so we simply add the new entry
                            topK.put(this_pop, parsed[1]);
                        else { // only insert if it belongs to top K, and then remove an element to keep it to K
                            Integer first = topK.firstKey();
                            if (this_pop > first) {
                                topK.remove(first);
                                topK.put(this_key, parsed[1]);
                            }
                        }
                    }
                }
            }
        }

        protected void cleanup(Mapper.Context context) throws IOException, InterruptedException {
            for (Map.Entry<Integer, String> entry: topK.entrySet()) {
                context.write(new IntWritable(entry.getKey()), new Text(entry.getValue()));
            }
        }
    }

    public static class TopKReducer extends Reducer<IntWritable, Text, IntWritable, Text> {
        private int K = 10;
        private TreeMap<Integer,String> topK;

        protected void setup(Context context) throws IOException, InterruptedException {
            K = (int) Long.parseLong(context.getConfiguration().get("K"));
            topK = new TreeMap<>();
        }

        private void add_this_to_topK(int key, Iterable<Text> values) {
            String this_value = "";

            for (Text value : values) {
                if (!this_value.isEmpty()) this_value += ", ";
                this_value += value.toString();
            }

            topK.put(key, this_value);
        }
        public void reduce(IntWritable key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
            /* A call to this method will have a population as key, and a list of cities
             * with that population in the values, so just combine that list into a single
             * string to add to our TreeMap
             */
            int this_pop = key.get();

            if (topK.size() < K) add_this_to_topK(this_pop, values);
            else {
                Integer first = topK.firstKey();
                if (this_pop > first) {
                    topK.remove(first);
                    add_this_to_topK(this_pop, values);
                }
            }
        }

        protected void cleanup(Context context) throws IOException, InterruptedException {
            // we want to print in descending order
            while (!topK.isEmpty()) {
                Integer key = topK.lastKey();
                String value = topK.get(key);
                topK.remove(key);
                context.write(new IntWritable(key), new Text(value));
            }
        }
    }

    public static class MonJob extends Configured implements Tool {
        public int run(String[] args) throws Exception {
            Configuration conf = getConf();
            conf.set("K", "10"); // here we could make K a parameter of the program (that was not asked for in the exercise)
            Job job = Job.getInstance(conf, "TopK");

            job.setNumReduceTasks(1);
            job.setJarByClass(TopK.class);
            job.setMapperClass(TopKMapper.class);
            job.setMapOutputKeyClass(IntWritable.class);
            job.setMapOutputValueClass(Text.class);
            job.setReducerClass(TopKReducer.class);
            job.setCombinerClass(TopKReducer.class);
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
