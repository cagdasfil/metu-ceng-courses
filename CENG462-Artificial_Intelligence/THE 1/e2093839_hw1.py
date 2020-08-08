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
            printpath()
            return "SUCCESS"
        if x in explored:
            indexofx = explored.index(x)
            if fchvalueofx[0] < fchofexplored[indexofx][0]:
                explored.append(x)
                fchofexplored.append(fchvalueofx)
                inserttopath(x, fchvalueofx)
                expandwithx(x, c)
        else:
            explored.append(x)
            fchofexplored.append(fchvalueofx)
            inserttopath(x, fchvalueofx)
            expandwithx(x, c)
    print("FAILURE")

def IDA():
    fmax = fchoffrontier[0][2]
    while True:
        (result,c) = LFS(fmax)
        if result == -1:
            printpath()
            break
        elif c > m:
            print("FAILURE",'','')
            break
        else:
            fmax = result

def LFS(fmax):
    fvalues = []
    costvalues = []
    del frontier[1:]
    del fchoffrontier[1:]
    i=0
    while True:
        if i == len(frontier):
            break
        elif frontier[i] == goal:
            path.append(goal)
            return (-1,0)
        elif fchoffrontier[i][0] > fmax:
            fvalues.append(fchoffrontier[i][0])
            costvalues.append(fchoffrontier[i][1])
            i+=1
        else:
            expandbacktrack(frontier[i], fchoffrontier[i][1], i+1)
            del path[fchoffrontier[i][1]:]
            path.append(frontier[i])
            i+=1
    return (min(fvalues),costvalues[fvalues.index(min(fvalues))])


def expandbacktrack(grid, c, i):
    newgrid = copy.deepcopy(grid)
    (x,y) = indexingrid(grid, "_")

    if y != 0:
        newgrid[x][y] = newgrid[x][y-1]
        newgrid[x][y-1] = "_"
        newh = h(newgrid, goal)
        frontier.insert(i,newgrid)
        fchoffrontier.insert(i,[c+1+newh, c+1, newh])
        newgrid = copy.deepcopy(grid)

    if y != n-1:
        newgrid[x][y] = newgrid[x][y+1]
        newgrid[x][y+1] = "_"
        newh = h(newgrid, goal)
        frontier.insert(i,newgrid)
        fchoffrontier.insert(i,[c+1+newh, c+1, newh])
        newgrid = copy.deepcopy(grid)

    if x != 0 :
        newgrid[x][y] = newgrid[x-1][y]
        newgrid[x-1][y] = "_"
        newh = h(newgrid, goal)
        frontier.insert(i,newgrid)
        fchoffrontier.insert(i,[c+1+newh, c+1, newh])
        newgrid = copy.deepcopy(grid)

    if x != n-1 :
        newgrid[x][y] = newgrid[x+1][y]
        newgrid[x+1][y] = "_"
        newh = h(newgrid, goal)
        frontier.insert(i,newgrid)
        fchoffrontier.insert(i,[c+1+newh, c+1, newh])
        newgrid = copy.deepcopy(grid)

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

def expandwithx(grid, c):
    newgrid = copy.deepcopy(grid)
    (x,y) = indexingrid(grid, "_")
    if x != n-1 :
        newgrid[x][y] = newgrid[x+1][y]
        newgrid[x+1][y] = "_"
        inserttofrontier(newgrid, c)
        newgrid = copy.deepcopy(grid)
    if x != 0 :
        newgrid[x][y] = newgrid[x-1][y]
        newgrid[x-1][y] = "_"
        inserttofrontier(newgrid, c)
        newgrid = copy.deepcopy(grid)
    if y != n-1:
        newgrid[x][y] = newgrid[x][y+1]
        newgrid[x][y+1] = "_"
        inserttofrontier(newgrid, c)
        newgrid = copy.deepcopy(grid)
    if y != 0:
        newgrid[x][y] = newgrid[x][y-1]
        newgrid[x][y-1] = "_"
        inserttofrontier(newgrid, c)
        newgrid = copy.deepcopy(grid)

def inserttofrontier(grid, c):
    newh = h(grid, goal)
    f = c+1+newh
    if frontier == []:
        frontier.append(grid)
        fchoffrontier.append([f, c+1, newh])
    else:
        i = 0
        lenn = len(fchoffrontier)
        while f >= fchoffrontier[i][0]:
            if f == fchoffrontier[i][0] and c+1 > fchoffrontier[i][1]:
                break
            if i == lenn-1:
                i+=1
                break
            i+=1
        frontier.insert(i, grid)
        fchoffrontier.insert(i, [f, c+1, newh])

def inserttopath(grid, fch):
    i = 0
    lenn = len(fchofpath)
    while i<lenn and fch[1] > fchofpath[i][1]:
        i+=1
    del path[i:]
    del fchofpath[i:]
    path.append(grid)
    fchofpath.append(fch)

def printpath():
    print("SUCCESS\n")
    for grid in path:
        for row in grid:
            for i in range(len(row)-1):
                print(row[i]),
            print(row[-1])
        if grid != path[-1]:
            print("")

####### MAIN ##############################

alg = raw_input()
m = eval(raw_input())
n = eval(raw_input())

frontier = []
explored = []
fchoffrontier = []
fchofexplored = []
fchofpath = []
path = []

tempgrid = []
for i in range(0,n):
    row = raw_input()
    tempgrid.append(row.split())

frontier.append(tempgrid)
path.append(tempgrid)

tempgrid = []
for i in range(0,n):
    row = raw_input()
    tempgrid.append(row.split())

goal = tempgrid[:]

fchoffrontier.append([f(0,h(goal, frontier[0])), 0, h(goal, frontier[0])])
fchofpath.append(fchoffrontier[0])

if alg=="A*":
    Astar()
elif alg=="IDA*":
    IDA()
