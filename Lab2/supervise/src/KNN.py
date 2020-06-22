import numpy as np


class KNN:

    def __init__(self, k=10):
        self.k = k
        self.x_train = None
        self.y_train = None

    def fit(self, x_train, y_train):
        self.x_train = x_train
        self.y_train = y_train
        return self
    
    def predict(self, x_test):
        y_test = [self._predict(x) for x in x_test]
        return np.array(y_test)
    
    def _predict(self, x):
        distances = [np.sqrt(np.sum((x_train - x)**2)) for x_train in self.x_train]
        nearest = np.argsort(distances)[:self.k]
        top_k = [self.y_train[index] for index in nearest]
        d = {}
        for i in top_k:
            if i not in d:
                d[i] = 0
            else:
                d[i] += 1
        return sorted(d.items(), key = lambda kv:(kv[1], kv[0]), reverse=True)[0][0]