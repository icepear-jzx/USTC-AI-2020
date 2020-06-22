import numpy as np


class PCA:
    
    def __init__(self, threshold=0.99):
        self.threshold = threshold
    
    def fit(self, X):
        X_T = X.T
        mean_X = X_T - np.mean(X_T, axis=1, keepdims=True)
        cov_X = np.dot(mean_X, mean_X.T)
        eig_val, eig_vec = np.linalg.eig(cov_X)
        val_index = np.argsort(eig_val)[::-1]
        total_val = np.sum(eig_val)
        temp = 0
        for i in range(len(eig_vec)):
            if temp / total_val >= self.threshold:
                break
            temp += eig_val[val_index[i]]
        vec_m = eig_vec[val_index[:i]]
        vec_m = vec_m.T
        return np.dot(X, vec_m)
