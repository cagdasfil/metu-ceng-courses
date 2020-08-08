import math
import copy

attributes = {
    "parents": ["usual","pretentious","great_pret"],
    "has_nurs": ["proper", "less_proper", "improper", "critical", "very_crit"],
    "form": ["complete", "completed", "incomplete", "foster"],
    "children": ["1", "2", "3", "more"],
    "housing": ["convenient", "less_conv", "critical"],
    "finance": ["convenient", "inconv"],
    "social": ["nonprob", "slightly_prob", "problematic"],
    "health": ["recommended", "priority", "not_recom"],
    "class": ["not_recom", "recommend", "very_recom", "priority", "spec_prior"]
}

def entropy(data_set):
    if not data_set:
        return 0
    class_counts = {"not_recom":0, "recommend":0, "very_recom":0, "priority":0, "spec_prior":0}
    for data in data_set:
        class_counts[data[8]]+=1
    total_entropy = 0.0
    for label in attributes["class"]:
        p_i = class_counts[label]/len(data_set)
        if p_i != 0:
            total_entropy -= p_i * math.log(p_i, 2)
    return total_entropy

def information_gain(data_set, index):
    attribute_values = list(attributes.values())[index]
    set_entropy = entropy(data_set)
    set_length = len(data_set)
    subset_dict = dict()
    for attribute in attribute_values:
        subset_dict[attribute] = []
    for data in data_set:
        subset_dict[data[index]].append(data)
    subset_entropy_total = 0
    for attribute in subset_dict:
        subset = subset_dict[attribute]
        subset_length = len(subset)
        if set_length != 0:
            subset_entropy_total += (subset_length/set_length)*entropy(subset)
    return set_entropy - subset_entropy_total

def gain_ratio(data_set, index):
    attribute_values = list(attributes.values())[index]
    set_length = len(data_set)
    subset_dict = dict()
    for attribute in attribute_values:
        subset_dict[attribute] = []
    for data in data_set:
        subset_dict[data[index]].append(data)
    ratio_total = 0
    for attribute in subset_dict:
        subset = subset_dict[attribute]
        subset_length = len(subset)
        if set_length != 0:
            ratio = subset_length / set_length
            if ratio != 0:
                ratio_total += ratio * math.log(ratio, 2)
    if ratio_total != 0:
        return -information_gain(data_set, index)/ratio_total
    else:
        return 0

def gini_index(data_set):
    if not data_set:
        return 1
    class_counts = {"not_recom":0, "recommend":0, "very_recom":0, "priority":0, "spec_prior":0}
    for data in data_set:
        class_counts[data[8]]+=1
    gini = 1.0
    for label in attributes["class"]:
        p_i = class_counts[label]/len(data_set)
        if p_i != 0:
            gini -= pow(p_i,2)
    return gini


def average_gini_index(data_set, index):
    attribute_values = list(attributes.values())[index]
    set_length = len(data_set)
    subset_dict = dict()
    for attribute in attribute_values:
        subset_dict[attribute] = []
    for data in data_set:
        subset_dict[data[index]].append(data)
    gini_total = 0.0
    for attribute in subset_dict:
        subset = subset_dict[attribute]
        subset_length = len(subset)
        if set_length != 0:
            ratio = subset_length / set_length
            if ratio != 0:
                gini_total += ratio * gini_index(subset)
    return gini_total

def split_data(data_set, index):
    attribute_values = list(attributes.values())[index]
    subset_dict = dict()
    for attribute in attribute_values:
        subset_dict[attribute] = []
    for data in data_set:
        subset_dict[data[index]].append(data)
    return subset_dict

def find_best_attribute(data_set, attribute_indexes, strategy_index):
    strategies = [information_gain, gain_ratio, average_gini_index]
    chosen_strategy = strategies[strategy_index]
    if strategy_index < 2:
        best = {"index":-1, "score":0}
        for index in attribute_indexes:
            score = chosen_strategy(data_set, index)
            if score > best["score"]:
                best["score"] = score
                best["index"] = index
        return best
    else:
        best = {"index":-1, "score":0.5}
        for index in attribute_indexes:
            score = chosen_strategy(data_set, index)
            if score < best["score"]:
                best["score"] = score
                best["index"] = index
        return best

class Node:
    def __init__(self, att_val, decision_index, data):
        self.att_val = att_val
        self.decision_index = decision_index
        self.children = []
        self.data = data
        self.predicted_class = "None"

class Tree:
    def __init__(self, data_set):
        self.root = Node("root", -1, data_set)

def predict_class(data_set):
    classes = ["not_recom", "recommend", "very_recom", "priority", "spec_prior"]
    if not data_set:
        return classes[0]
    class_counts = {"not_recom":0, "recommend":0, "very_recom":0, "priority":0, "spec_prior":0}
    for data in data_set:
        class_counts[data[8]]+=1
    maxx = [0, "None"]
    for att in classes:
        if class_counts[att] > maxx[0]:
            maxx = [class_counts[att], att]
    return maxx[1]

def build_tree(node, strategy_index):
    attribute_indexes = list(range(0,8))
    return build_tree_recursively(node, attribute_indexes, strategy_index)

def build_tree_recursively(node, attribute_indexes, strategy_index):
    if attribute_indexes==[]:
        node.predicted_class = predict_class(node.data)
        return node
    best_att = find_best_attribute(node.data, attribute_indexes, strategy_index)
    if best_att["index"]==-1:
        node.predicted_class = predict_class(node.data)
        return node
    attribute_values = list(attributes.values())[best_att["index"]]
    node.decision_index = best_att["index"]
    subset_dict = dict()
    for attribute in attribute_values:
        subset_dict[attribute] = []
    for data in node.data:
        subset_dict[data[best_att["index"]]].append(data)
    for attribute in attribute_values:
        node.children.append(Node(attribute, -1, subset_dict[attribute]))
    new_attribute_indexes = copy.deepcopy(attribute_indexes)
    new_attribute_indexes.remove(best_att["index"])
    for i in range(len(node.children)):
        node.children[i] = build_tree_recursively(node.children[i], new_attribute_indexes, strategy_index)
    return node

def traverse_tree(data, root):
    if root.predicted_class != "None":
        return root.predicted_class
    if root.decision_index != -1:
        att_val = data[root.decision_index]
        for child in root.children:
            if att_val == child.att_val:
                return traverse_tree(data, child)

def test_decision_tree(data_set, root):
    correct = 0
    for data in data_set:
        if data[8] == traverse_tree(data, root):
            correct += 1
    return (correct*100)/len(data_set)

def count_nodes(node):
    count = 1
    for child in node.children:
        count += count_nodes(child)
    return count

def read_data(file_path):
    file = open(file_path, "r")
    data_set = []
    data = file.readline()
    while data:
        data = data.strip().split(",")
        data_set.append(data)
        data = file.readline()
    return data_set

def main():

    train_file_path = "./train.txt"
    test_file_path = "./test.txt"

    train_set = read_data(train_file_path)
    test_set = read_data(test_file_path)

    tree = Tree(train_set)

    # second argument of build_tree function is for selection/pruning strategy
    #   0 : Information Gain
    #   1 : Gain Ratio
    #   2 : Average Gini Index
    decision_tree = build_tree(tree.root, 0)

    print("Accuracy:", test_decision_tree(test_set, decision_tree))
    print("Number of Nodes:", count_nodes(decision_tree))

if __name__ == '__main__':
    main()