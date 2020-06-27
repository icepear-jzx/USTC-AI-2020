import numpy as np
from cvxopt import matrix as cvxopt_matrix
from cvxopt import solvers as cvxopt_solvers


class SVM:

    def __init__(self, args):
        KERNEL_MAP = {
            'LINEAR': self.linear_kernel,
            'RBF': self.rbf_kernel
        }

        self.soft = args.soft
        self.sigma = args.sigma
        self.epsilon = args.epsilon
        self.kernel = KERNEL_MAP[args.kernel]
        self.w = self.b = self.sv_a = self.sv_x = self.sv_y = None
    
    def linear_kernel(self, x, y):
        return np.dot(x, y.T)
    
    def rbf_kernel(self, x, y):
        return np.exp(-np.sqrt(np.linalg.norm(x - y) ** 2 / (2 * self.sigma ** 2)))
    
    def fit(self, x_train, y_train):
        m = x_train.shape[0]
        y_train = y_train.reshape(-1, 1) * 1.
        if self.kernel == self.linear_kernel:
            x_dash = y_train * x_train
            H = np.dot(x_dash , x_dash.T) * 1.
        else:
            K = np.zeros((m, m))
            for i in range(m):
                for j in range(m):
                    K[i,j] = self.kernel(x_train[i], x_train[j])
            H = np.outer(y_train, y_train) * K

        P = cvxopt_matrix(H)
        q = cvxopt_matrix(-np.ones((m, 1)))
        G = cvxopt_matrix(np.vstack((np.eye(m) * -1, np.eye(m))))
        h = cvxopt_matrix(np.hstack((np.zeros(m), np.ones(m) * self.soft)))
        A = cvxopt_matrix(y_train.reshape(1, -1))
        b = cvxopt_matrix(np.zeros(1))

        cvxopt_solvers.options['show_progress'] = False
        cvxopt_solvers.options['abstol'] = 1e-10
        cvxopt_solvers.options['reltol'] = 1e-10
        cvxopt_solvers.options['feastol'] = 1e-10

        sol = cvxopt_solvers.qp(P, q, G, h, A, b)
        alphas = np.array(sol['x'])

        sv = (alphas > self.epsilon).flatten()
        ind = np.arange(len(alphas))[sv]
    
        self.sv_a = alphas[sv]
        self.sv_x = x_train[sv]
        self.sv_y = y_train[sv]
        
        if self.kernel == self.linear_kernel:
            self.w = ((y_train * alphas).T @ x_train).reshape(-1, 1)
            self.b = np.mean(y_train[sv] - np.dot(x_train[sv], self.w))
        else:
            self.b = 0
            for n in range(len(self.sv_a)):
                self.b += self.sv_y[n]
                self.b -= np.sum(self.sv_a * self.sv_y * K[ind[n], sv])
            self.b /= len(self.sv_a)

    def predict(self, x_test):
        if self.kernel == self.linear_kernel:
            return np.sign(np.dot(x_test, self.w) + self.b).astype(int).flatten()
        else:
            m = x_test.shape[0]
            y_pred = np.zeros(m)
            for i in range(m):
                s = 0
                for sv_a, sv_y, sv_x in zip(self.sv_a, self.sv_y, self.sv_x):
                    s += sv_a * sv_y * self.kernel(x_test[i], sv_x)
                y_pred[i] = s
            return np.sign(y_pred + self.b).astype(int)
