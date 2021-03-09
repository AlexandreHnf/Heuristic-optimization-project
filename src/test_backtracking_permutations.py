wct = 0

def backtrackingPermutations(nb, s=[]):
    global wct
    if (len(s) == nb):
        print(s, end="  |  ")
    else:
        # solutions candidates
        for i in range(nb):
            if (i not in s):
                s.append(i)
                backtrackingPermutations(nb, s)
                s.pop()
    return "done"

backtrackingPermutations(3)