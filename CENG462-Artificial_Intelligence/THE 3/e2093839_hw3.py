def theorem_prover(clauses, theorem_list):
    theorem = theorem_list[0]
    neg_theorem = negate(theorem)
    list_of_resolutions = []
    while True:
        found = False
        neg_theorem = negate(theorem)
        for clause in clauses:
            if isIncluded(neg_theorem, clause):
                newClause = replace(neg_theorem, clause)
                eliminatedClause = eliminate(neg_theorem, newClause)
                list_of_resolutions.append(theorem + "$" + clause + "$" + eliminatedClause)
                theorem = eliminatedClause
                if theorem == "empty":
                    return "yes",list_of_resolutions
                found = True
            else:
                pass
        if not found:

            break

    return "no", []

def negate(clause):
    items = clause.split('+')

    if items == []:
        return ""

    if items[0][0] == '~':
        newItems = items[0][1:]
    else:
        newItems = '~'+items[0]

    for item in items[1:]:
        if item[0] == '~':
            newItems += '+'+item[1:]
        else:
            newItems += '+~'+item[1:]
    return newItems

def parse(clause): # return(functions, variables, constants)
    tempClause = clause[:]
    functions = []
    variables = []
    constants = []
    ops = ['(',')',',','+','~']

    while True:
        index = tempClause.find('(')
        if index == -1:
            break
        elif index == 0:
            tempClause = tempClause[1:]
        else:
            if tempClause[index-1] not in ops:
                functions.append(tempClause[index-1])
                tempClause = tempClause[index+1:]
            else:
                tempClause = tempClause[index+1:]

    for token in clause:
        if token.islower() and token not in functions:
            variables.append(token)
        elif token.isupper():
            constants.append(token)

    return functions, variables, constants

def isIncluded(neg_theorem, clause):
    parsed_neg_theorem = parse(neg_theorem)
    parsed_clause = parse(clause)
    if parsed_neg_theorem[0][0] in parsed_clause[0]:
        index1 = neg_theorem.find(parsed_neg_theorem[0][0])
        index2 = clause.find(parsed_neg_theorem[0][0])
        if index1==-1 or index2==-1:
            return False
        if index1==0 and index2==0:
            return True
        if index1==0:
            if clause[index2-1]=='~':
                return False
            else:
                return True
        if index2==0:
            if neg_theorem[index1-1]=='~':
                return False
            else:
                return True
        else:
            if neg_theorem[index1-1]=='~' and clause[index2-1]=='~':
                return True
            else:
                return False

def replace(neg_theorem, clause):
    parsed_neg_theorem = parse(neg_theorem)
    parsed_clause = parse(clause)
    newClause = clause[:]
    i=0
    for x in parsed_neg_theorem[2]:
        if x in parsed_clause[2]:
            continue
        else:
            newClause = newClause.replace(parsed_clause[1][i],x)
            i+=1
    return newClause

def eliminate(neg_theorem, newClause):
    newestClause = newClause.replace(neg_theorem, "")
    if newestClause == "":
        return "empty"
    if "++" in newestClause:
        newestClause = newestClause.replace("++", "+")
    if newestClause[0] == '+':
        newestClause = newestClause[1:]
    if newestClause[-1] == '+':
        newestClause = newestClause[:len(newestClause) - 1]
    return newestClause

print theorem_prover(["p(A,f(t))", "q(z)+~p(z,f(B))", "~q(y)+r(y)"],["~r(A)"])

print theorem_prover(["p(A,f(t))", "q(z)+~p(z,f(B))", "q(y)+r(y)"],["~r(A)"]) 