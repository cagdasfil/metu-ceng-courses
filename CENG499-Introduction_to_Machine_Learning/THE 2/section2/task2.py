import matplotlib.pyplot as plt
import copy

def HAC(k, data_set):

    def euclidean_distance(data1, data2):
        dist = 0.0
        for i in range(len(data1)):
            dist += pow(data1[i] - data2[i], 2)
        return pow(dist, 0.5)

    def single_length(cluster1, cluster2):
        min_dist = 99999999999
        for point1 in cluster1:
            for point2 in cluster2:
                distance = euclidean_distance(point1, point2)
                if distance < min_dist:
                    min_dist = distance
        return min_dist

    def complete_length(cluster1, cluster2):
        max_dist = 0.0
        for point1 in cluster1:
            for point2 in cluster2:
                distance = euclidean_distance(point1, point2)
                if distance > max_dist:
                    max_dist = distance
        return max_dist

    def average_length(cluster1, cluster2):
        total_dist = 0
        for point1 in cluster1:
            for point2 in cluster2:
                distance = euclidean_distance(point1, point2)
                total_dist += distance
        return total_dist/(len(cluster1)*len(cluster2))

    def centroid_length(cluster1, cluster2):
        total_point1 = [0, 0]
        for point1 in cluster1:
            total_point1[0] += point1[0]
            total_point1[1] += point1[1]
        total_point2 = [0, 0]
        for point2 in cluster2:
            total_point2[0] += point2[0]
            total_point2[1] += point2[1]
        center1 = [ total_point1[0]/len(cluster1), total_point1[1]/len(cluster1) ]
        center2 = [ total_point2[0]/len(cluster2), total_point2[1]/len(cluster2) ]
        return euclidean_distance(center1, center2)

    def cluster_data(raw_data, criterion_index):
        data = copy.deepcopy(raw_data)
        criterions = [single_length, complete_length, average_length, centroid_length]
        cluster_count = len(data)
        while cluster_count != k :
            min_len = [0, 0, 99999999999]
            for i in range(len(data)-1):
                for j in range(i+1,len(data)):
                    length = criterions[criterion_index](data[i], data[j])
                    if length < min_len[2]:
                        min_len = [i, j, length]
            data[min_len[0]] += data[min_len[1]]
            del data[min_len[1]]
            cluster_count = len(data)
        return data

    def cluster_plot_data(data_set):
        fig, axs = plt.subplots(2, 2)
        axs[0, 0].set_title('single length linkage')
        axs[0, 1].set_title('complete length linkage')
        axs[1, 0].set_title('average length linkage')
        axs[1, 1].set_title('centroid length linkage')
        x = []
        y = []
        plt_colors = []
        colors = ["red", "blue", "green", "gold", "maroon", "aqua", "lime", "chocolate"]
        for i in range(4):
            clustered_data = cluster_data(data_set, i)
            for c in range(len(clustered_data)):
                cluster = clustered_data[c]
                color = colors[c]
                for data in cluster:
                    x.append(data[0])
                    y.append(data[1])
                plt_colors += [color]*len(cluster)
            axs[int(i/2), i%2].scatter(x, y, c=plt_colors)
        plt.show()

    cluster_plot_data(data_set)

def read_data_set(file_path):
    data = []
    data_file = open(file_path, "r")
    data_file_line = data_file.readline()
    while data_file_line:
        str_data_line = data_file_line.split(" ")
        data_line = list()
        data_line.append(float(str_data_line[0]))
        data_line.append(float(str_data_line[1].strip()))
        data.append([data_line])
        data_file_line = data_file.readline()
    return data

def main():

    file_path = "./dataset1.txt"
    data_set = read_data_set(file_path)
    HAC(2, data_set)

    file_path = "./dataset2.txt"
    data_set = read_data_set(file_path)
    HAC(2, data_set)

    file_path = "./dataset3.txt"
    data_set = read_data_set(file_path)
    HAC(2, data_set)

    file_path = "./dataset4.txt"
    data_set = read_data_set(file_path)
    HAC(4, data_set)

if __name__ == '__main__':
    main()