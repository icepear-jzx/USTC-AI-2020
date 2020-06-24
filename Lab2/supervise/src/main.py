import argparse
import numpy as np
import csv
from sklearn.preprocessing import LabelEncoder
from KNN import KNN
from LR import LR
from SVM import SVM


DATA_PATH_MAP = {
    'MAT': '../data/student-mat.csv',
    'POR': '../data/student-por.csv',
}

MODEL_MAP = {
    'KNN': KNN,
    'SVM': SVM,
    'LR': LR,
}

STR_COLS = [0, 1, 3, 4, 5, 8, 9, 10, 11, 15, 16, 17, 18, 19, 20, 21, 22]
NUM_COLS = [2, 6, 7, 12, 13, 14, 23, 24, 25, 26, 27, 28, 29, 30, 31]


def load_data(data_path, model, useG):
    y = []
    x = [[] for _ in range(32)]
    with open(data_path, 'r') as f:
        lines = csv.reader(f, delimiter=';')
        lines.__next__()
        for line in lines:
            y.append(line[-1])
            for i in range(32):
                x[i].append(line[i])
    
    if model == 'SVM':
        y = [1 if int(row) >= 10 else -1 for row in y]
    else:
        y = [1 if int(row) >= 10 else 0 for row in y]

    for i in NUM_COLS:
        x[i] = [int(ele) for ele in x[i]]
    for i in STR_COLS:
        x[i] = LabelEncoder().fit_transform(x[i])

    y = np.array(y).reshape(-1, 1)
    x = np.array(x).T

    if not useG:
        x = x[:, :-2]

    return x, y


def split_data(x, y, train_size=0.7):
    data = np.concatenate((x, y), axis=-1)
    np.random.shuffle(data)
    x = data[:, :-1]
    y = data[:, -1]

    train_num = int(x.shape[0] * train_size)
    x_train = x[:train_num]
    y_train = y[:train_num]
    x_test = x[train_num:]
    y_test = y[train_num:]
    
    return x_train, y_train, x_test, y_test


def evaluate(y_true, y_pred):
    TP = FP = FN = TN = 0
    for i in range(len(y_true)):
        if y_true[i] == 1 and y_pred[i] == 1:
            TP += 1
        elif y_true[i] == 1 and y_pred[i] == 0:
            FN += 1
        elif y_true[i] == 0 and y_pred[i] == 1:
            FP += 1
        else:
            TN += 1
    P = TP / (TP + FP)
    R = TP / (TP + FN)
    F = 2 * P * R / (P + R)
    return P, R, F


if __name__ == '__main__':
    
    parser = argparse.ArgumentParser()
    parser.add_argument('--model', type=str, choices=['KNN', 'SVM', 'LR'], default='KNN')
    parser.add_argument('--kernel', type=str, choices=['LINEAR', 'RBF'], default='LINEAR')
    parser.add_argument('--dataset', type=str, choices=['MAT', 'POR'], default='MAT')
    parser.add_argument('--useG', type=bool, default=False)
    parser.add_argument('--k', type=int, default=10)
    args = parser.parse_args()

    x, y = load_data(DATA_PATH_MAP[args.dataset], args.model, args.useG)
    x_train, y_train, x_test, y_test = split_data(x, y)
    model = MODEL_MAP[args.model](args)
    model.fit(x_train, y_train)
    y_pred = model.predict(x_test) 
    print(evaluate(y_test, y_pred))
