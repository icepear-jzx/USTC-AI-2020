import numpy as np


class LR:

    def __init__(self, args):
        self.epochs = args.epochs
        self.alpha = args.alpha
        self.weight = None

    def fit(self, x_train, y_train) :
        self.weight = np.ones((x_train.shape[1],1))
        x_train = np.mat(x_train)
        y_train = np.mat(y_train).T
        for _ in range(self.epochs):
            h = self.sigmoid(x_train * self.weight)
            error = y_train - h
            self.weight += self.alpha * x_train.T * error
    
    def predict(self, x_test):
        y_pred = self.sigmoid(np.mat(x_test) * self.weight)
        y_pred = [1 if x[0] > 0.5 else 0 for x in y_pred]
        return y_pred

    def sigmoid(self, x):
        return 1 / (1 + np.exp(-x))
