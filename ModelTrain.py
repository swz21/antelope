# -*- coding: utf-8 -*-
### load module
from sklearn.model_selection import train_test_split
from xgboost import XGBClassifier
from sklearn.metrics import accuracy_score
import numpy as np
import pickle
import datetime
from sklearn.preprocessing import LabelEncoder
import os
import argparse

os.environ['CUDA_VISIBLE_DEVICES'] = '0'  # 指定要使用的GPU设备编号，如果有多个GPU可选择其中一个
os.environ['XGB_USE_CUDA'] = '1'  # 启用XGBoost的CUDA支持

parser = argparse.ArgumentParser()
parser.add_argument('--cca', type=str, default='cubic', help='cca name')
args = parser.parse_args()
CCA = args.cca
# CCA = "bbr"

DATAPATH = "/home/sunwz/antelope/train/" + CCA
MODELPATH = "/home/sunwz/antelope/traindata/" + CCA + ".pickle"


def loadData(dir):
    data = []
    target = []
    fileName = dir
    try:
        rawData = np.loadtxt(fileName)
        for item in rawData:
            data.append(item[:-1])
            target.append(item[-1])
    except  Exception as e:
        pass

    return np.array(data), np.array(target)


### load datasets
data, target = loadData(DATAPATH)
#
# ### data analysis
print(data.shape)
print(target.shape)

### data split
x_train, x_test, y_train, y_test = train_test_split(data,
                                                    target,
                                                    test_size=0.2,
                                                    random_state=33)

# encode y as label
le = LabelEncoder()
y_train_label = le.fit_transform(y_train)
y_test_label = le.fit_transform(y_test)

### fit model for train data
model = XGBClassifier(n_estimators=40, max_depth=3, subsample=1, gamma=1, learning_rate=0.15, tree_method='gpu_hist')
model.fit(x_train, y_train_label)
with open(MODELPATH, "wb") as fw:
    pickle.dump(model, fw)

