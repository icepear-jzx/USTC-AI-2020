import numpy as np


class PCA:
    
    def __init__(self, args):
        self.threshold = args.threshold
    
    def fit(self, x):
        x = x - x.mean(axis=0)
        cov = np.cov(x.T) / x.shape[0]
        w, v = np.linalg.eig(cov)
        idx = np.argsort(w)[::-1]
        w = w[idx]
        v = v[:, idx]
        i = 0
        while np.sum(w[:i]) / np.sum(w) < self.threshold and i < len(w):
            i += 1
        return np.dot(x, v[:, :i])
