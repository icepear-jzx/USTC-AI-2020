import numpy as np
import csv
from sklearn.preprocessing import LabelEncoder
from KNN import KNN
from LogisticRegression import LogisticRegression as LR
from SVM import SVM


MAT_DATASET = '../data/student/student-mat.csv'
POR_DATASET = '../data/student/student-por.csv'


def load_data(filename=MAT_DATASET, algorithm='KNN', use_G = True):
    labels = []
    attributes = [[] for i in range(32)]
    with open(filename, 'r') as f:
        lines = csv.reader(f, delimiter=';')
        next(lines) # first line
        for line in lines:
            labels.append(line[-1])
            for i in range(32):
                attributes[i].append(line[i])
    if algorithm == 'SVM':
        labels = list(map(lambda x: 1 if int(x) >= 10 else -1, labels))
    else:
        labels = list(map(lambda x: 1 if int(x) >= 10 else 0, labels))
    for i in [0, 1, 3, 4, 5, 8, 9, 10, 11, 15, 16, 17, 18, 19, 20, 21, 22]:
        attributes[i] = LabelEncoder().fit_transform(attributes[i])
    for i in range(len(attributes)):
        attributes[i] = list(map(lambda x: int(x), attributes[i]))
    x = np.array(attributes).T
    if not use_G:
        x = x[:, :-2]
    return x, labels


def train_test_split(x, y, test_size=0.3):
    # shuffle
    x = np.array(x)
    y = np.array(y).reshape(-1, 1)
    data = np.concatenate((x, y), axis=-1)
    np.random.shuffle(data)
    x = data[:, :-1]
    y = data[:,-1]

    # split
    train_num = int(x.shape[0]*(1-test_size))
    x_train = np.array(x[:train_num])
    y_train = np.array(y[:train_num])
    x_test = np.array(x[train_num:])
    y_test = np.array(y[train_num:])
    return x_train, y_train, x_test, y_test


def evaluate(y_true, y_pred):
    TP = FP = FN = TN = 0
    for i in range(len(y_true)):
        if y_true[i] == 1:
            if y_pred[i] == 1:
                TP += 1
            else:
                FN += 1
        else:
            if y_pred[i] == 1:
                FP += 1
            else:
                TN += 1
    P = TP / (TP + FP)
    R = TP / (TP + FN)
    return P, R, 2 * P * R / (P + R)


if __name__ == '__main__':
    x, y = load_data(POR_DATASET, algorithm='SVM')
    x_train, y_train, x_test, y_test = train_test_split(x, y)
    # model = KNN()
    # model = LR()
    model = SVM(kernel='rbf')
    model.fit(x_train, y_train)
    y_pred = model.predict(x_test) 
    # print(y_pred)
    print(evaluate(y_test, y_pred))
