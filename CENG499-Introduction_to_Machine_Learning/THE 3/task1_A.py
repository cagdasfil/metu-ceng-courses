from random import seed
import numpy as np
import matplotlib.pyplot as plt
from sklearn.svm import SVC

# fix randomness - DO NOT CHANGE/REMOVE THIS
seed(1234)
np.random.seed(1234)

data = np.load("task1\\task1_A.npz")
X, y = data["X"], data["y"]

clfs = [
    ["Linear", SVC(kernel="linear")],
    ["Sigmoid", SVC(kernel="sigmoid")],
    ["Polynomial", SVC(kernel="poly")],
    ["RBF", SVC(kernel="rbf")]
]

h = 0.02

max_vals = np.amax(X, axis=0)
min_vals = np.amin(X, axis=0)

x_min = min_vals[0]-1
x_max = max_vals[0]+1
y_min = min_vals[1]-1
y_max = max_vals[1]+1

x_mesh, y_mesh = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))

x_mesh_min = x_mesh.min()
x_mesh_max = x_mesh.max()
y_mesh_min = y_mesh.min()
y_mesh_max = y_mesh.max()

for kernel_name, clf in clfs:
    clf.fit(X,y)
    accuracy = clf.score(X,y)
    Z = clf.predict(np.c_[x_mesh.ravel(), y_mesh.ravel()])
    Z = Z.reshape(x_mesh.shape)
    plt.xlabel("First Feature")
    plt.ylabel("Second Feature")
    plt.title(kernel_name+" Kernel - Accuracy: "+str(accuracy))
    plt.contourf(x_mesh, y_mesh, Z, cmap=plt.cm.coolwarm, alpha=0.8)
    plt.scatter(X[:, 0], X[:, 1], edgecolors="black", s=20, c=y, cmap=plt.cm.coolwarm)
    plt.xlim(x_mesh_min, x_mesh_max)
    plt.ylim(y_mesh_min, y_mesh_max)
    plt.yticks(())
    plt.xticks(())
    plt.show()