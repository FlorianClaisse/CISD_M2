import pandas as pd
import rpy2.robjects as ro
from rpy2.robjects import pandas2ri

TRAIN_PATH = "./data/spam_data_train.rda"
TEST_PATH = "./data/spam_data_test.rda"

pandas2ri.activate()

ro.r["load"](TRAIN_PATH)
objects = ro.r["ls"]()
print(objects)

train_data = ro.r["data_train"]

train_df = pandas2ri.rpy2py(train_data)
print(train_df.head())