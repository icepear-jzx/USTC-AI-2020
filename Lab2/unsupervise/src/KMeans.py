import numpy as np


class KMeans():

    def __init__(self, k=3):
        self.k = k

    def _distance(self, x, y):
        return np.sqrt(np.sum((x -  y) ** 2))
    
    def _evaluate(self, x, center, cluster):
        nearest = {}
        for i in range(len(center)):
            min_dist = 10000000
            for j in range(len(center)):
                if j == i:
                    continue
                dist = self._distance(center[i], center[j])
                if dist < min_dist:
                    nearest[i] = j
                    min_dist = dist
        # 计算轮廓系数
        s = []
        for i in range(len(cluster)):
            b = []
            a = []
            for j in range(len(cluster)):
                if j == i:
                    continue
                if cluster[i] == cluster[j]:
                    a.append(self._distance(x[i], x[j]))
                elif cluster[j] == nearest[cluster[i]]:
                    b.append(self._distance(x[i], x[j]))
            mean_b = np.mean(np.array(b))
            mean_a = np.mean(np.array(a))
            s.append((mean_b - mean_a) / max(mean_b, mean_a))
        s = np.mean(np.array(s))
        print('s: ', s)
        return s

    def fit(self, X):
        # 随机选取K个中心
        center = np.zeros((self.k, X.shape[1]))
        for i in range(self.k):
            index = np.random.randint(X.shape[0])
            center[i] = X[index]
        clusterChanged = True
        cluster = np.zeros((X.shape[0]), dtype='int32')
        while clusterChanged:
            clusterChanged = False
            # 聚类
            for i in range(X.shape[0]):
                min_dist = 10000000
                min_index = 0
                for j in range(self.k):
                    dist = self._distance(X[i], center[j])
                    if dist < min_dist:
                        min_dist = dist
                        min_index = j
                if min_index != cluster[i]:
                    cluster[i] = min_index
                    clusterChanged = True
            # 更新中心
            for j in range(self.k):
                center[j] = np.mean(X[cluster == j], axis=0)
        s = self._evaluate(X, center, cluster)
        return cluster, s

        

