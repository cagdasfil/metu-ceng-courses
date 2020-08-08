import pickle, gzip
import sys
import numpy as np

def sigmoid(x):
    return 1.0/(1.0+np.exp(-x))

def sigmoid_derivation(x):
    return np.exp(x)/pow((np.exp(x)+1), 2)

def activation_function(x):
    if x<=-1:
        return 0.0
    elif -1<x<1:
        return x/2.0+0.5
    else:
        return 1.0

def activation_function_derivation(x):
    if x<=-1:
        return 0
    elif -1<x<1:
        return 0.5
    else:
        return 0

def loss_function(true_labels, predicted_labels, n):
    total = 0.0
    for i in range(0,n):
        total += pow(float(true_labels[i])-float(predicted_labels[i]), 2)
    return total/n

def main():

    def update():
        diff = train_labels[tr_data] - output1

        for h in range(n_h):
            error = -diff * activation_function_derivation(output0)
            grad_h_o = error * hidden1[h]

            for i in range(n_i):
                grad_i_h = error * weights2[h] * sigmoid_derivation(hidden0[h]) * train_data[tr_data, i]
                weights1[i, h] -= l_rate * grad_i_h

            weights2[h] -= l_rate * grad_h_o

    def test():
        correct = 0
        for te_data in range(number_of_test_data):
            for h in range(n_h):
                hidden0[h] = np.dot(test_data[te_data], weights1[:, h])
                hidden1[h] = activation_function(hidden0[h])

            output0 = np.dot(hidden1, weights2)
            output1 = sigmoid(output0)

            if output1 > 0.5:
                result = 1
            else:
                result = 0

            if result == test_labels[te_data]:
                correct += 1
        return correct * 100 / number_of_test_data

    with gzip.open(sys.argv[1]) as train_set:
        train_data, train_labels = pickle.load(train_set, encoding="latin1")

    with gzip.open(sys.argv[2]) as test_set:
        test_data, test_labels = pickle.load(test_set, encoding="latin1")

    number_of_train_data = len(train_data)
    number_of_test_data = len(test_data)

    number_of_epochs = int(sys.argv[3])

    np.random.seed(1234)

    n_i = len(train_data[0])
    n_o = 1
    n_h = int((n_i+n_o)/2.0+1)

    weights1 = np.random.rand(n_i, n_h)*0.01
    weights2 = np.random.rand(n_h)*0.01

    hidden0 = np.zeros(n_h)
    hidden1 = np.zeros(n_h)

    l_rate = 0.1

    for e in range(number_of_epochs):
        for tr_data in range(number_of_train_data):
            for h in range(n_h):
                hidden0[h] = np.dot(train_data[tr_data], weights1[:, h])
                hidden1[h] = activation_function(hidden0[h])

            output0 = np.dot(hidden1, weights2)
            output1 = sigmoid(output0)

            update()

    accuracy = test()
    print("Accuracy:", accuracy)


if __name__ == '__main__':
    main()