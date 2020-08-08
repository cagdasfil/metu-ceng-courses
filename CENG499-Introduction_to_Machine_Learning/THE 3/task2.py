from random import seed

# fix randomness - DO NOT CHANGE/REMOVE THIS
seed(1234)

def linear_regression(train_path, test_path, num_epochs, learning_rate):
    """
    Performs multivariate regression
    :param train_path: path of the training set, a string
    :param test_path: path of the test set, a string
    :param num_epochs: the number of epochs, an integer
    :param learning_rate: learning rate, a float
    :return: RMSE (Root Mean Square Error) of the test set
    """
    def read_data(path):
        file = open(path, "r")
        X_data = []
        y_data = []
        file_line = file.readline()
        while file_line:
            data_line = file_line.strip().split(",")
            for i in range(len(data_line)):
                data_line[i] = float(data_line[i])
            X_data.append(data_line[:-1])
            y_data.append(data_line[-1])
            file_line = file.readline()
        return X_data, y_data

    def normalize(data_set):
        data_length = len(data_set[0])
        max_values = [-9999999999999] * data_length
        min_values = [9999999999999] * data_length
        for data in data_set:
            for i in range(data_length):
                if data[i] > max_values[i]:
                    max_values[i] = data[i]
                if data[i] < min_values[i]:
                    min_values[i] = data[i]
        for data in data_set:
            for i in range(data_length):
                data[i] = (data[i] - min_values[i]) / (max_values[i] - min_values[i])
        return data_set

    def matrix_mult(m1, m2):
        total=0
        for i in range(len(m1)):
            total+=m1[i]*m2[i]
        return total

    def add_bias(data_set):
        biased_data = []
        for data in data_set:
            biased_data.append([1]+data)
        return biased_data

    def train(data_set, num_epochs, learning_rate):
        m = len(data_set)
        n = len(data_set[0])
        theta = [0] * n
        for epoch in range(num_epochs):
            for i in range(m):
                y = matrix_mult(X_train_biased[i], theta)
                for j in range(n):
                    theta[j] = theta[j] - 2 * learning_rate * (y - y_train[i]) * X_train_biased[i][j]
        return theta

    def calculate_RMSE(X, theta, y):
        total = 0
        for i in range(len(X)):
            y_pred = matrix_mult(X[i], theta)
            total += (y_pred - y[i]) ** 2
        return pow((total / len(y)), 0.5)


    X_train, y_train = read_data(train_path)
    X_test, y_test = read_data(test_path)

    normalize(X_train)
    normalize(X_test)

    X_train_biased = add_bias(X_train)

    theta = train(X_train_biased, num_epochs, learning_rate)

    X_test_biased = add_bias(X_test)

    return calculate_RMSE(X_test_biased, theta, y_test)
