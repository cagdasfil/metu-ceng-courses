from random import seed

# fix randomness; DO NOT CHANGE THIS
seed(1234)


def naive_bayes(train_path, test_path):
    """
    Performs naive Bayes classification
    :param train_path: path of the training set, a string
    :param test_path: path of the test set, a string
    :return: percent accuracy value for the test set, e.g., 85.43
    """
    def read_train_data():
        train_file = open(train_path,"r")
        lines = train_file.readlines()
        train_file.close()
        train_X = []
        train_y = []
        for line in lines:
            data = line.strip().split(",")
            train_X.append(data[:-1])
            train_y.append(data[-1])
        return train_X, train_y

    def read_test_data():
        test_file = open(test_path,"r")
        lines = test_file.readlines()
        test_file.close()
        test_X = []
        test_y = []
        for line in lines:
            data = line.strip().split(",")
            test_X.append(data[:-1])
            test_y.append(data[-1])
        return test_X, test_y

    def get_features():
        features = []
        feature_count = len(train_X[0])
        for col in range(feature_count):
            features.append([])
            for data in train_X:
                if data[col] not in features[col]:
                    features[col].append(data[col])
            for data in test_X:
                if data[col] not in features[col]:
                    features[col].append(data[col])
        return features

    def get_labels():
        labels = []
        for label in train_y:
            if label not in labels:
                labels.append(label)
        for label in test_y:
            if label not in labels:
                labels.append(label)
        return labels

    def get_counts():
        counts = []
        for feature in features:
            newFeature = dict()
            for value in feature:
                newValue = dict()
                for label in labels:
                    newValue[label]=0
                newFeature[value]=newValue
            counts.append(newFeature)
        for i in range(len(train_X)):
            data = train_X[i]
            label = train_y[i]
            for feature in range(len(data)):
                counts[feature][data[feature]][label]+=1
        return counts

    def get_label_counts():
        label_counts = {}
        for label in labels:
            label_counts[label]=0
        for label in train_y:
            label_counts[label]+=1
        return label_counts

    def test_model():
        data_count = len(test_X)
        correct = 0
        p_labels = [0]*len(labels)
        for i in range(data_count):
            for label in range(len(labels)):
                p_label = 1
                for feature in range(len(test_X[i])):
                    count = feature_counts[feature][test_X[i][feature]][labels[label]]
                    p = count/label_counts[labels[label]]
                    p_label = p_label*p
                p_labels[label] = p_label*(label_counts[labels[label]]/data_count)
            max_p = max(p_labels)
            predicted = p_labels.index(max_p)
            if test_y[i] == labels[predicted]:
                correct += 1
        return correct*100/data_count

    train_X, train_y = read_train_data()
    test_X, test_y = read_test_data()

    features = get_features()
    labels = get_labels()

    feature_counts = get_counts()
    label_counts = get_label_counts()

    accuracy = test_model()

    return accuracy