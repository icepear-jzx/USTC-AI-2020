import numpy as np
import csv
import matplotlib.pyplot as plt
from PCA import PCA
from KMeans import KMeans


def load_data(filename):
    data = [[] for _ in range(13)]
    with open(filename, 'r') as f:
        lines = csv.reader(f, delimiter=',')
        for line in lines:
            for i in range(13):
                data[i].append(float(line[i]))
    data  = np.array(data).T
    labels = data[:,0]
    x = data[:, 1:]
    # min-max标准化
    min_x = np.min(x, axis=0)
    max_x = np.max(x, axis=0)
    x = (x - min_x) / (max_x - min_x)
    # print(labels.shape)
    # print(x.shape)
    return x, labels


def evaluate(cluster, y):
    # 兰德系数
    a = b = c = d = 0
    for i in range(len(y)):
        for j in range(len(y)):
            if i == j:
                continue
            if y[i] == y[j]:
                if cluster[i] == cluster[j]:
                    a += 1
                else:
                    b += 1
            else:
                if cluster[i] == cluster[j]:
                    c += 1
                else:
                    d += 1
    r = (a + d) / (a + b + c + d)
    return r


def visualize(pca_x, cluster):
    from sklearn.manifold import TSNE
    import pandas as pd
    tsne = TSNE()
    y = np.array(cluster).reshape(-1, 1)
    x = pd.DataFrame(np.concatenate((pca_x, y), axis=-1))
    a = tsne.fit_transform(x)
    data = pd.DataFrame(a,index=x.index)
    d1 = data[x[2]==0]
    d2 = data[x[2]==1]
    d3 = data[x[2]==2]
    plt.plot(d1[0],d1[1],'r.',d2[0],d2[1],'go',d3[0],d3[1],'b*')
    plt.show()


if __name__ == '__main__': 
    dataset_dir = '../input/wine.data'
    x, y = load_data(dataset_dir)
    model = PCA(threshold=0.5)
    pca = model.fit(x)
    print('dim: ', pca.shape[1])
    kmodel = KMeans()
    cluster, s = kmodel.fit(pca)
    print('r: ', evaluate(cluster, y))
    visualize(pca, cluster)
    np.savetxt('../output/output.csv', cluster, delimiter=',', fmt='%d')
