import random

random.seed(1234)  # fix randomness


def kNN(k, train_set, test_set):
    """
    the unweighted k-NN algorithm using Euclidean distance as the metric

    :param k: the k value, i.e, how many neighbors to consider
    :param train_set: training set, a list of lists where each nested list is a training instance
    :param test_set: test set, a list of lists where each nested list is a test instance
    :return: percent accuracy for the test set, e.g., 78.42
    """

    def euclidean_distance(data1, data2):
        dist = 0.0
        for i in range(len(data1)-1):
            dist += pow(data1[i] - data2[i], 2)
        return pow(dist, 0.5)

    def nearest_neighbors(k, data):
        """
        :param k: number of neighbor to consider
        :param data: data instance as list
        :return: class values of the k nearest neighbors as list
        """
        distances = []
        for i in range(len(train_set)):
            distances.append([i, euclidean_distance(data,train_set[i])])
        distances.sort(key = lambda x: x[1])
        classes = []
        for i in range(k):
            classes.append(train_set[distances[i][0]][-1])
        return classes

    def predict_class(classes):
        """
        :param classes: class values as list
        :return: most occurred class in classes list
        """
        return max(set(classes), key=classes.count)

    def test_accuracy():
        """
        :return: percentage of the accuracy of the model on test set
        """
        correct = 0.0
        for test_data in test_set:
            neighbors = nearest_neighbors(k, test_data)
            if predict_class(neighbors) == test_data[-1]:
                correct += 1
        return (correct * 100) / len(test_set)

    return test_accuracy()

def find_best_k(train_set, test_set, num_folds):
    """
    finds the best k value by using K-fold cross validation. Try at least 10 different k values. Possible choices
    can be: 1, 3, 5, 7, 9, 11, 13, 15, 17, 19. Besides the return value, as a side effect, print each k value and
    the corresponding validation accuracy to the screen as a tuple. As an example,
    (1, 78.65)
    (3, 79.12)
    ...
    (19, 76.99)

    :param train_set: training set, a list of lists where each nested list is a training instance
    :param test_set: test set, a list of lists where each nested list is a test instance
    :param num_folds: the K value in K-fold cross validation
    :return: a tuple, best k value and percent accuracy for the test set using the best k value, e.g., (3, 80.06)
    """
    fold_length = int(len(train_set)/num_folds)
    results = []
    for k in range(1,21,2):
        total_accuracy = 0.0
        for f in range(num_folds):
            index1 = f*fold_length
            index2 = index1+fold_length
            # data between index1 and index2 is used as test set, rest of the data is used as train set
            validation_accuracy = kNN(k, train_set[:index1]+train_set[index2:], train_set[index1:index2])
            total_accuracy += validation_accuracy
        avg_accuracy = total_accuracy/num_folds
        results.append((k, avg_accuracy))
        print(results[-1])
    results.sort(key = lambda x: x[1])
    best_k = results[-1][0]
    return best_k, kNN(best_k, train_set, test_set)

"""
def read_data(file_path):
    file = open(file_path, "r")
    data_load = []
    data = file.readline()
    while data:
        data = data.strip().split(",")
        for i in range(len(data) - 1):
            data[i] = float(data[i])
        data_load.append(data)
        data = file.readline()
    return data_load

def normalize(data_set):
    data_length = len(data_set[0])-1
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
            data[i] = (data[i]-min_values[i])/(max_values[i]-min_values[i])
    return data_set

def main():

    train_file_path = "./task1_train.txt"
    test_file_path = "./task1_test.txt"

    train_set = read_data(train_file_path)
    test_set = read_data(test_file_path)

    # calling the function kNN
    k=3
    test_accuracy=kNN(k, train_set, test_set)
    print("k: {} | test accuracy: {}".format(k, test_accuracy))

    # calling the function find_best_k
    num_folds = 5
    best_k, test_accuracy = find_best_k(train_set, test_set, num_folds)
    print("best k is {} | test accuracy: {}".format(best_k, num_folds))

    # calling the function kNN with normalized data
    normalized_train_set =  normalize(train_set)
    normalized_test_set = normalize(test_set)
    k = 13
    test_accuracy = kNN(k, normalized_train_set, normalized_test_set)
    print("k: {} | test accuracy: {}".format(k, test_accuracy))
    
if __name__ == '__main__':
    main()
"""