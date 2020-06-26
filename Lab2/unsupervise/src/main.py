import argparse
import numpy as np
import csv
import matplotlib.pyplot as plt
from PCA import PCA
from KMeans import KMeans


DATA_PATH = '../input/wine.data'


def load_data(data_path):
    y = []
    x = [[] for _ in range(13)]
    with open(data_path, 'r') as f:
        lines = csv.reader(f, delimiter=',')
        for line in lines:
            y.append(int(line[0]))
            for i in range(13):
                x[i].append(float(line[i+1]))
    x = np.array(x).T
    y = np.array(y)
    return x, y

def normalize(x):
    min_x = np.min(x, axis=0)
    max_x = np.max(x, axis=0)
    x = (x - min_x) / (max_x - min_x)
    return x


def evaluate(cluster, y):
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
    import pandas as pd
    y = np.array(cluster).reshape(-1, 1)
    x = pd.DataFrame(np.concatenate((pca_x, y), axis=-1))
    data = pd.DataFrame(x, index=x.index)
    d1 = data[x[2]==0]
    d2 = data[x[2]==1]
    d3 = data[x[2]==2]
    plt.plot(d1[0], d1[1], 'r.', d2[0], d2[1], 'gx', d3[0], d3[1], 'b*')
    plt.show()


if __name__ == '__main__': 

    parser = argparse.ArgumentParser()
    parser.add_argument('--usePCA', type=str, default='False', choices=['True', 'False'])
    parser.add_argument('--k', type=int, default=3)
    parser.add_argument('--threshold', type=float, default=0.5)
    args = parser.parse_args()

    x, y = load_data(DATA_PATH)
    x = normalize(x)

    if args.usePCA == 'True':
        x = PCA(args).fit(x)

    cluster, s = KMeans(args).fit(x)
    r = evaluate(cluster, y)
    print('s:', s)
    print('r:', r)

    if args.usePCA == 'False':
        x = PCA(args).fit(x)
    visualize(x, cluster)

    np.savetxt('../output/output.csv', cluster, delimiter=',', fmt='%d')
