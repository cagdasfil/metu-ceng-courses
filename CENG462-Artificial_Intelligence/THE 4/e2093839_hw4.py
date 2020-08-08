import sys
import random
import copy



def valueIteration():


    def nextStep(x,y,M,N,oldUtilities):
        values = []

        if y-1 < 0:  # outside
            values.append(oldUtilities[y][x])
        else:
            values.append(oldUtilities[y-1][x])

        if x+1 >= N:  # outside
            values.append(oldUtilities[y][x])
        else:
            values.append(oldUtilities[y][x+1])

        if y+1 >= M:  # outside
            values.append(oldUtilities[y][x])
        else:
            values.append(oldUtilities[y+1][x])

        if x-1 < 0:  # outside
            values.append(oldUtilities[y][x])
        else:
            values.append(oldUtilities[y][x-1])

        return values

    theta = float(inputFile.readline().rstrip())
    gamma = float(inputFile.readline().rstrip())
    MN = inputFile.readline().rstrip().split(" ")
    M = int(MN[0])
    N = int(MN[1])

    obstacles = []
    numberOfObstacles = int(inputFile.readline().rstrip())
    for i in range(numberOfObstacles):
        obs = inputFile.readline().rstrip().split(" ")
        obstacles.append([int(obs[0]), int(obs[1])])

    pitfalls = []
    numberOfPitfalls = int(inputFile.readline().rstrip())
    for i in range(numberOfPitfalls):
        pit = inputFile.readline().rstrip().split(" ")
        pitfalls.append([int(pit[0]), int(pit[1])])

    goal = inputFile.readline().rstrip().split(" ")
    goal[0] = int(goal[0])
    goal[1] = int(goal[1])

    rewardValues = inputFile.readline().rstrip().split(" ")
    r_d = float(rewardValues[0])
    r_o = float(rewardValues[1])
    r_p = float(rewardValues[2])
    r_g = float(rewardValues[3])


    utilities = [[0.0 for col in range(N)] for row in range(M)]
    rewards = [[r_d for col in range(N)] for row in range(M)]

    for c in obstacles:
        rewards[M-c[1]][c[0]-1] = r_o

    for c in pitfalls:
        rewards[M-c[1]][c[0]-1] = r_p
        utilities[M - c[1]][c[0] - 1] = r_p

    rewards[M-goal[1]][goal[0]-1] = r_g
    utilities[M - goal[1]][goal[0] - 1] = r_g

    while True:
        oldUtilities = copy.deepcopy(utilities)
        delta = 0
        for x in range(N):
            for y in range(M):
                if [x+1, M-y] in obstacles or [x+1, M-y] == goal or [x+1, M-y] in pitfalls:
                    continue
                utilities[y][x] = rewards[y][x] + gamma * max(nextStep(x, y, M, N, oldUtilities))
                diff = abs(utilities[y][x] - oldUtilities[y][x])
                if diff > delta:
                    delta = diff
        if delta < theta:
            break

    for x in range(N):
        for y in range(M-1, -1, -1):
            if [x + 1, M - y] in obstacles or [x + 1, M - y] == goal or [x + 1, M - y] in pitfalls:
                outputFile.write(str(x+1)+" "+str(M-y)+" "+"0\n")
                continue
            adjacents = nextStep(x, y, M, N, utilities)
            maxVal = max(adjacents)
            outputFile.write(str(x + 1) + " " + str(M - y) + " " + str(adjacents.index(maxVal)) + "\n")

def QLearning():

    def nextStep(x, y, M, N, oldUtilities):
        values = []

        if y - 1 < 0:  # outside
            values.append(oldUtilities[y][x])
        else:
            values.append(oldUtilities[y - 1][x])

        if x + 1 >= N:  # outside
            values.append(oldUtilities[y][x])
        else:
            values.append(oldUtilities[y][x + 1])

        if y + 1 >= M:  # outside
            values.append(oldUtilities[y][x])
        else:
            values.append(oldUtilities[y + 1][x])

        if x - 1 < 0:  # outside
            values.append(oldUtilities[y][x])
        else:
            values.append(oldUtilities[y][x - 1])

        return values

    numberOfEpisode = int(inputFile.readline().rstrip())
    alpha = float(inputFile.readline().rstrip())
    gamma = float(inputFile.readline().rstrip())
    epsilon = float(inputFile.readline().rstrip())
    MN = inputFile.readline().rstrip().split(" ")
    M = int(MN[0])
    N = int(MN[1])

    obstacles = []
    numberOfObstacles = int(inputFile.readline().rstrip())
    for i in range(numberOfObstacles):
        obs = inputFile.readline().rstrip().split(" ")
        obstacles.append([int(obs[0]), int(obs[1])])

    pitfalls = []
    numberOfPitfalls = int(inputFile.readline().rstrip())
    for i in range(numberOfPitfalls):
        pit = inputFile.readline().rstrip().split(" ")
        pitfalls.append([int(pit[0]), int(pit[1])])

    goal = inputFile.readline().rstrip().split(" ")
    goal[0] = int(goal[0])
    goal[1] = int(goal[1])

    rewardValues = inputFile.readline().rstrip().split(" ")
    r_d = float(rewardValues[0])
    r_o = float(rewardValues[1])
    r_p = float(rewardValues[2])
    r_g = float(rewardValues[3])

    utilities = [[0.0 for col in range(N)] for row in range(M)]
    rewards = [[r_d for col in range(N)] for row in range(M)]

    for c in obstacles:
        rewards[M - c[1]][c[0] - 1] = r_o

    for c in pitfalls:
        rewards[M - c[1]][c[0] - 1] = r_p
        utilities[M - c[1]][c[0] - 1] = r_p

    rewards[M - goal[1]][goal[0] - 1] = r_g
    utilities[M - goal[1]][goal[0] - 1] = r_g

    while True:
        oldUtilities = copy.deepcopy(utilities)
        delta = 0
        for x in range(N):
            for y in range(M):
                if [x + 1, M - y] in obstacles or [x + 1, M - y] == goal or [x + 1, M - y] in pitfalls:
                    continue
                utilities[y][x] = rewards[y][x] + gamma * max(nextStep(x, y, M, N, oldUtilities))
                diff = abs(utilities[y][x] - oldUtilities[y][x])
                if diff > delta:
                    delta = diff
        if delta < epsilon:
            break

    for x in range(N):
        for y in range(M - 1, -1, -1):
            if [x + 1, M - y] in obstacles or [x + 1, M - y] == goal or [x + 1, M - y] in pitfalls:
                outputFile.write(str(x + 1) + " " + str(M - y) + " " + "0\n")
                continue
            adjacents = nextStep(x, y, M, N, utilities)
            maxVal = max(adjacents)
            outputFile.write(str(x + 1) + " " + str(M - y) + " " + str(adjacents.index(maxVal)) + "\n")

inputFile = open(sys.argv[1], "r")
outputFile = open(sys.argv[2], "w")

type = inputFile.readline()

if type.rstrip() == 'V':
    valueIteration()
elif type.rstrip() == 'Q':
    QLearning()
else:
    print "Wrong type"