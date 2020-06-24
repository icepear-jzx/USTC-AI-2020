import numpy as np


class KNN:

    def __init__(self, args):
        self.k = args.k
        self.x_train = None
        self.y_train = None

    def fit(self, x_train, y_train):
        self.x_train = x_train
        self.y_train = y_train
    
    def predict(self, x_test):
        y_pred = []
        for attr_test in x_test:
            distances = [self.euclidean_distance(attr_test, attr_train) for attr_train in self.x_train]
            top_k_index = np.argsort(distances)[:self.k]
            top_k_value = self.y_train[top_k_index]
            values, counts = np.unique(top_k_value, return_counts=True)
            y_pred.append(values[np.argmax(counts)])
        return np.array(y_pred)
    
    def euclidean_distance(self, x1, x2):
        return np.sqrt(np.sum(np.power(x1 - x2, 2)))