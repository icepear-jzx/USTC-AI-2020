import numpy as np


class LogisticRegression:

    def __init__(self, iter=100, alpha=0.001):
        self.iter = iter
        self.alpha = alpha

    def _sigmoid(self, x):
        return 1 / (1 + np.exp(-x))

    def fit(self, x_train, y_train) :
        self.weight = np.ones((x_train.shape[1],1))
        x_train = np.mat(x_train)
        y_train = np.mat(y_train).T
        for _ in range(self.iter):
            h = self._sigmoid(x_train * self.weight)
            error = y_train - h
            self.weight += self.alpha * x_train.T * error
    
    def predict(self, x_test):
        y_pred = self._sigmoid(np.mat(x_test) * self.weight)
        y_pred = list(map(lambda x: 1 if int(x[0]) > 0.5 else 0, y_pred))
        return y_pred



    
