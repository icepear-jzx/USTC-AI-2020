import numpy as np


class KMeans():

    def __init__(self, args):
        self.k = args.k

    def fit(self, x):
        idx = np.random.choice(len(x), size=self.k, replace=False)
        centers = x[idx]
        while True:
            cluster = self.cluster_center(x, centers)
            new_centers = self.new_centers(x, cluster)
            if (centers == new_centers).all():
                break
            else:
                centers = new_centers
        s = self.silhouette_coef(x, cluster)
        return cluster, s

    def cluster_center(self, points, centers):
        distances = np.sqrt(((points - centers[:, np.newaxis]) ** 2).sum(axis=2))
        return np.argmin(distances, axis=0)
    
    def new_centers(self, points, cluster):
        return np.array([points[cluster==k].mean(axis=0) for k in range(self.k)])
    
    def silhouette_coef(self, x, cluster):
        n = cluster.shape[0]
        A = np.array([self.intra_cluster_distance(x, cluster, i) for i in range(n)])
        B = np.array([self.nearest_cluster_distance(x, cluster, i) for i in range(n)])
        return np.mean(np.nan_to_num((B - A) / np.maximum(A, B)))

    def intra_cluster_distance(self, x, cluster, i):
        idx = np.where(cluster == cluster[i])[0]
        if len(idx) == 0:
            return 0
        a = np.mean([self.euclidean_distance(x[i], x[j]) for j in idx])
        return a

    def nearest_cluster_distance(self, x, cluster, i):
        b = np.min([
                np.mean([self.euclidean_distance(x[i], x[j]) for j in np.where(cluster == c)[0]]) 
            for c in set(cluster) if not c == cluster[i]])
        return b
    
    def euclidean_distance(self, x1, x2):
        return np.sqrt(np.sum(np.power(x1 - x2, 2)))
