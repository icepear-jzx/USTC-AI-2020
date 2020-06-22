import numpy as np


class SVM:

    def __init__(self, kernel='linear', C=100, epsilon=0.00001, sigma=5.0):
        self.C = C
        if kernel == 'linear':
            self.kernel = self._linear_kernel
        elif kernel == 'rbf':
            self.kernel = self._rbf_kernel
        else:
            print('Invalid kernel: {}'.format(kernel))
            exit(-1)
        self.epsilon = epsilon
        self.sigma = sigma
        self.alpha = np.array([])
        self.b = 0
        self.m = 0
        self.n = 0
        self.k = np.array([])
        self.y_train = np.array([])
        self.x_train = np.array([])
    
    def _linear_kernel(self, x, y):
        return np.dot(x, y.T)
    
    def _rbf_kernel(self, x, y):
        return np.exp(-np.sqrt(np.linalg.norm(x-y) ** 2 / (2 * self.sigma ** 2)))
                
    def _cal_g(self, i):
        index = [i for i, alpha in enumerate(self.alpha) if alpha != 0]
        gxi = 0
        for j in index:
            gxi += self.alpha[j] * self.y_train[j] * self.k[i, j]
        gxi += self.b
        return gxi
    
    def _is_satisfy_KKT(self, i):
        yi = self.y_train[i]
        gxi = self._cal_g(i)
        if (np.fabs(self.alpha[i]) < self.epsilon) and (yi * gxi >= 1 - self.epsilon):
            return True
        elif (np.fabs(self.alpha[i] - self.C) < self.epsilon) and (yi * gxi <= 1 + self.epsilon):
            return True
        elif (self.alpha[i] > -self.epsilon) and (self.alpha[i] < (self.C + self.epsilon)) \
                and (np.fabs(yi * gxi - 1) < self.epsilon):
            return True
        return False

    def _cal_Ei(self, i):
        gxi = self._cal_g(i)
        return gxi - self.y_train[i]

    def fit(self, x_train, y_train, max_iter=100):
        self.m, self.n = x_train.shape
        self.alpha = np.zeros(self.m)
        self.x_train = x_train
        self.y_train = y_train 
        self.k = np.zeros((self.m, self.m))

        for i in range(self.m):
            for j in range(i, self.n):
                self.k[i, j] = self.k[j, i] = self.kernel(self.x_train[i], self.x_train[j]) 
        self.E = []
        for i in range(self.m):
            self.E.append(-self.y_train[i])

        iteration = 0
        flag = 0
        while flag == 0:
            iteration += 1
            # print(iteration)
            flag = 1
            for i in range(self.m):
                # choose the first alpha
                if self._is_satisfy_KKT(i) == False:
                    E1 = self.E[i]
                    maxdiff = -1
                    for s in range(self.m):
                        tempdiff = np.fabs(self.E[s] - E1)
                        if tempdiff > maxdiff:
                            maxdiff = tempdiff
                            E2 = self.E[s]
                            j = s
                    if maxdiff == -1:
                        continue

                    y1 = self.y_train[i]
                    y2 = self.y_train[j]
                    alpha_old_1 = self.alpha[i]
                    alpha_old_2 = self.alpha[j]

                    if y1 != y2:
                        L = max(0, alpha_old_2 - alpha_old_1)
                        H = min(self.C, self.C + alpha_old_2 - alpha_old_1)
                    else:
                        L = max(0, alpha_old_2 + alpha_old_1 - self.C)
                        H = min(self.C, alpha_old_2 + alpha_old_1)
                    if L == H:
                        continue
                    
                    k11 = self.k[i, i]
                    k22 = self.k[j, j]
                    k21 = self.k[j, i]
                    k12 = self.k[i, j]
                    eta = k11 + k22 - 2 * k12
                    if eta <= 0:
                        continue
                    alpha_new_2 = alpha_old_2 + y2 * (E1 - E2) / (k11 + k22 - 2 * k12)
                    if alpha_new_2 < L:
                        alpha_new_2 = L
                    elif alpha_new_2 > H:
                        alpha_new_2 = H

                    alpha_new_1 = alpha_old_1 + y1 * y2 * (alpha_old_2 - alpha_new_2)

                    b1_new = -1 * E1 - y1 * k11 * (alpha_new_1 - alpha_old_1) \
                            - y2 * k21 * (alpha_new_2 - alpha_old_2) + self.b
                    b2_new = -1 * E2 - y1 * k12 * (alpha_new_1 - alpha_old_1) \
                            - y2 * k22 * (alpha_new_2 - alpha_old_2) + self.b
                    if (alpha_new_1 > 0) and (alpha_new_1 < self.C):
                        b_new = b1_new
                    elif (alpha_new_2 > 0) and (alpha_new_2 < self.C):
                        b_new = b2_new
                    else:
                        b_new = (b1_new + b2_new) / 2

                    if (np.fabs(alpha_new_2 - alpha_old_2) < self.epsilon ** 2) \
                        and (np.fabs(alpha_new_1 - alpha_old_1) < self.epsilon ** 2):
                        continue
                    else:
                        flag = 0
                    
                    self.alpha[i] = alpha_new_1
                    self.alpha[j] = alpha_new_2
                    self.b = b_new
                    self.E[i] = self._cal_Ei(i)
                    self.E[j] = self._cal_Ei(j)

    def predict(self, x_test):
        y_test = [self._predict(x) for x in x_test]
        return y_test
    
    def _predict(self, x):
        index = [i for i, alpha in enumerate(self.alpha) if alpha > 0]
        r = 0
        for j in index:
            r += self.alpha[j] * self.y_train[j] * self.kernel(x, self.x_train[j])
        r += self.b
        r = np.sign(r)
        return r
