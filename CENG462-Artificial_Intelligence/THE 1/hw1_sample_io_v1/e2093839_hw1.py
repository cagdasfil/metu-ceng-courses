import copy

def Astar():
    while frontier != []:
        x = frontier.pop(0)
        fchvalueofx = fchoffrontier.pop(0)
        c = fchvalueofx[1]
        if c > m:
            continue
        if x == goal:
            path.append(x)
            return "SUCCESS"
        if x in explored:
            indexofx = explored.index(x)
            if fchvalueofx[0] < fchofexplored[indexofx][0]:
                explored.append(x)
                fchofexplored.append(fchvalueofx)
                expandwithx(x)
        else:
            explored.append(x)
            fchofexplored.append(fchvalueofx)
    return "FAILURE"

def IDA():
    pass

def h(grid, goal):
    manhattan = 0
    for i in range(1,n**2):
        (x1,y1) = indexingrid(grid, str(i))
        (x2,y2) = indexingrid(goal, str(i))
        manhattan += abs(x1-x2) + abs(y1-y2)
    return manhattan

def f(c, h):
    return c+h

def indexingrid(grid, k):
    for i, j in enumerate(grid):
        if k in j:
            return (i, j.index(k))

def expandwithx(grid):
    newgrid = grid[:]
    (x,y) = indexingrid(grid, "_")
    if x != n-1 :
        newgrid[x][y] = newgrid[x+1][y]
        newgrid[x+1][y] = "_"
        print(newgrid)
        inserttofrontier(newgrid)
        newgrid = grid[:]
        print(newgrid)
    if x != 0 :
        newgrid[x][y] = newgrid[x-1][y]
        newgrid[x-1][y] = "_"
        print(newgrid)
        inserttofrontier(newgrid)
        newgrid = grid[:]
        print(newgrid)
    if y != n-1:
        newgrid[x][y] = newgrid[x][y+1]
        newgrid[x][y+1] = "_"
        print(newgrid)
        inserttofrontier(newgrid)
        newgrid = grid[:]
        print(newgrid)
    if y != 0:
        newgrid[x][y] = newgrid[x][y-1]
        newgrid[x][y-1] = "_"
        print(newgrid)
        inserttofrontier(newgrid)
        newgrid = grid[:]
        print(newgrid)

def inserttofrontier(grid):
    pass

####### MAIN ##############################

alg = raw_input()
m = eval(raw_input())
n = eval(raw_input())

frontier = []
explored = []
fchoffrontier = []
fchofexplored = []

tempgrid = []
for i in range(0,n):
    row = raw_input()
    tempgrid.append(row.split())

frontier.append(tempgrid)

tempgrid = []
for i in range(0,n):
    row = raw_input()
    tempgrid.append(row.split())

goal = tempgrid[:]

fchoffrontier.append([f(0,h(goal, frontier[0])), 0, h(goal, frontier[0])])

expandwithx(goal)

'''
if alg=="A*":
    Astar()
elif alg=="IDA*":
    IDA()
'''
