import math
import numpy as np
import pandas as pd
'''
Read input file to get (x,y) coordinates of points. After the data is read from the file, it should return the data points
as a list of tuples.Each data point is denoted with a tuple (x,y) where "x" denotes the x-coordinate and "y" denotes 
the y-coordinate.
'''
def read_input_file(input_file_name):
    # TODO: Implement here
    input_file = open(input_file_name, "r")
    lines = input_file.readlines()
    data_points = []
    for line in lines:
        points_str = line[line.find("(") + 1:line.find(")")]
        points = points_str.split(",")
        data_points.append((float(points[0]), float(points[1])))
    return data_points


class AgglomerativeClustering:
    LINKAGE = {
        'MIN': 0,
        'MAX': 1,
        'GROUP_AVG':2
    }

    def __init__(self,input_data):
        self.data=input_data 
        self.clusters_current=[]    
    # Add your member variables(if any here)
        self.proximity_matrix = []

    '''
    Explanations for AgglomerativeClustering class members:

    data:                   list of tuples including input 2-D data points. 
                            Each data point is denoted with a tuple (x,y) where "x" denotes the x-coordinate and 
                            "y" denotes the y-coordinate. Ex:[(1,2),(3,4),(100,101),....]
    clusters_current:       list of lists which includes the current elements of each cluster as tuples in a list and
                            where each data point is a member of one of the clusters. 
                            Ex:[[(1,2),(3,4),..],[(100,101),(100,102),....]]

    
    *** Make sure you keep template structure that is given to you. 
    *** YOU CAN ADD TO THE template STRUCTURE, BUT CAN NOT REMOVE FROM IT. ***
    '''

    '''
    This method is the first step of the algorithm: Initialize all "n" different data points to "n" different clusters.
    This "n" clusters are kept as list of lists. This method will convert each element of self.clusters_current attribute
    into a separate list and assign the new list into self.clusters_current attribute.
    Ex:  Before this method is invoked, self.clusters_current=[(1,2),(3,4),(5,6),(7,8)]
         After this method is invoked, self.clusters_current= [[(1,2)],[(3,4)],[(5,6)],[(7,8)]]
    '''
    
    
    def initialize_clustering(self):
        # TODO: Implement here
        for data in self.data:
            self.clusters_current.append([data])


    '''This method returns the resultant clusters as the result of the clustering given the linkage. The method should
    exit the loop of the algorithm and return the resultant clusters when the number of clusters is equal the given 
    number_of_clusters argument. In order to clarify, the following example is given: Assume input data consists of 100 data points,
    number_of_clusters=3 and linkage is "MIN". Then "fit_predict" should return a list of lists where each list denotes an 
    independent cluster and includes the data points(a tuple with x,y coordinates) inside each cluster.
    Ex: [[(0,2),(1,3),...],[(10,20),(11,21),(12,22),...],[(100,100),(101,100),(102,100),...]]
    '''

    def fit_predict(self,number_of_clusters,linkage):
        # TODO: Implement here
        self.initialize_proximity_matrix()
        while len(self.clusters_current) != number_of_clusters:
            if linkage == self.LINKAGE['MIN']:
                coordinate = self.merge()
                self.update_min(coordinate)
            elif linkage == self.LINKAGE['MAX']:
                coordinate = self.merge()
                self.update_max(coordinate)
            else:
                coordinate = self.merge()
                self.update_avg(coordinate)
        return self.clusters_current

    def euclidean(self, point1, point2):
        return (((point1[0] - point2[0]) ** 2 + (point1[1] - point2[1]) ** 2) ** 0.5)

    def initialize_proximity_matrix(self):
        for point1 in self.clusters_current:
            row = []
            for point2 in self.clusters_current:
                distance = self.euclidean(point1[0], point2[0])
                row.append(round(distance * 100) / 100.0)
            self.proximity_matrix.append(row)

    def merge(self):
        min = self.proximity_matrix[1][0]
        coordinate = (1, 0)
        for x in range(0, len(self.proximity_matrix)):
            for y in range(0, x):
                if self.proximity_matrix[x][y] <= min:
                    min = self.proximity_matrix[x][y]
                    coordinate = (x, y)
        return coordinate

    def update_min(self, coordinate):
        x = coordinate[0]
        y = coordinate[1]
        self.clusters_current[x] = self.clusters_current[x] + self.clusters_current[y]
        del self.clusters_current[y]
        for i in range(0, len(self.proximity_matrix)):
            if self.proximity_matrix[x][i] < self.proximity_matrix[y][i]:
                pass
            else:
                self.proximity_matrix[x][i] = self.proximity_matrix[y][i]

        del self.proximity_matrix[y]

        for j in range(0, len(self.proximity_matrix)):
            del self.proximity_matrix[j][y]

    def update_max(self, coordinate):
        x = coordinate[0]
        y = coordinate[1]
        self.clusters_current[x] = self.clusters_current[x] + self.clusters_current[y]
        del self.clusters_current[y]
        for i in range(0, len(self.proximity_matrix)):
            if self.proximity_matrix[x][i] > self.proximity_matrix[y][i]:
                pass
            else:
                self.proximity_matrix[x][i] = self.proximity_matrix[y][i]

        del self.proximity_matrix[y]

        for j in range(0, len(self.proximity_matrix)):
            del self.proximity_matrix[j][y]

    def update_avg(self, coordinate):
        x = coordinate[0]
        y = coordinate[1]

        self.clusters_current[x] = self.clusters_current[x] + self.clusters_current[y]
        del self.clusters_current[y]

        del self.proximity_matrix[y]

        for j in range(0, len(self.proximity_matrix)):
            del self.proximity_matrix[j][y]

        x = x - 1

        for i in range(0, len(self.proximity_matrix)):
            m1 = len(self.clusters_current[x])
            m2 = len(self.clusters_current[i])
            total = 0
            for j in range(0, m1):
                for k in range(0, m2):
                    total += self.euclidean(self.clusters_current[x][j], self.clusters_current[i][k])
            self.proximity_matrix[x][i] = round((total / (m1 * m2)) * 100) / 100.0
            self.proximity_matrix[i][x] = round((total / (m1 * m2)) * 100) / 100.0