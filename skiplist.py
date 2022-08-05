from random import randint
from math import log

class Lnode(object):
    def __init__(self, key):
        self.key = key
        h = 1
        while randint(0, 1) == 0: h += 1
        self.levels = [None for _ in range(h)]
        self.ptrlen = [1 for _ in range(h)] 

class SetSkiplist(object):
    def __init__(self, capacity = int(1e10), low_lim = -int(1e10)):
        self.head = Lnode(low_lim)
        max_h = int(log(capacity,2))+2
        self.head.levels = [None for _ in range(max_h)]
        self.head.ptrlen = [1 for _ in range(max_h)]
    
    def last_before(self, t):
        cur = self.head
        rank = 0
        while True:
            ok = False
            for i in range(len(cur.levels)-1,-1,-1):
                if cur.levels[i] and cur.levels[i].key < t:
                    rank += cur.ptrlen[i]
                    cur = cur.levels[i]
                    ok = True
                    break
            if not ok: break
        return cur, rank
    
    def rank(self, t):
        cur, rank = self.last_before(t)
        if cur.levels[0] and cur.levels[0].key == t:
            return rank
        return -1
    
    def how_many_smaller_than(self, t):
        cur, rank = self.last_before(t)
        return rank 
    
    def key_with_rank(self, r):
        if r<0: raise ValueError('Rank cannot be negative')
        cur = self.head
        rank = 0
        while True:
            ok = False
            for i in range(len(cur.levels)-1,-1,-1):
                if cur.levels[i] and rank + cur.ptrlen[i] < r+1:
                    rank += cur.ptrlen[i]
                    cur = cur.levels[i]
                    ok = True
                    break 
            if not ok: break 
        if rank + cur.ptrlen[0] == r+1 and cur.levels[0]: return cur.levels[0].key
        raise ValueError('There is no key with this rank')
    
    def search(self, t):
        return self.rank(t) != -1
    
    def path_to(self, t):
        cur = self.head
        path = []
        while True:
            path.append(cur)
            ok = False
            for i in range(len(cur.levels)-1,-1,-1):
                if cur.levels[i] and cur.levels[i].key < t:
                    cur = cur.levels[i]
                    ok = True
                    break
            if not ok: break     
        return path
        
    def add(self, t):
        path = self.path_to(t)
        cur = Lnode(t)
        k = 0
        tot_ptrlen = 1
        for j in range(len(path)-1,-1,-1):
            if k>0: tot_ptrlen += path[j].ptrlen[k-1]
            for i in range(k, min(len(path[j].levels), len(cur.levels))):
                cur.ptrlen[i] = 1 + path[j].ptrlen[i] - tot_ptrlen
                path[j].ptrlen[i] = tot_ptrlen
                
                cur.levels[i] = path[j].levels[i]
                path[j].levels[i] = cur
            for i in range(max(k, len(cur.levels)), len(path[j].levels)):
                path[j].ptrlen[i]+=1
            k = len(path[j].levels)
            
    def erase(self, t):
        path = self.path_to(t)
        cur = path[len(path)-1].levels[0]
        if cur is None: return False
        if cur.key != t: return False
        k = 0 
        for j in range(len(path)-1,-1,-1):
            for i in range(k, min(len(path[j].levels), len(cur.levels))):
                path[j].levels[i] = cur.levels[i]
                path[j].ptrlen[i] += cur.ptrlen[i] - 1
            for i in range(max(k, len(cur.levels)), len(path[j].levels)):
                path[j].ptrlen[i]-=1 
            k = len(path[j].levels)
        del cur
        return True

mylist = SetSkiplist()
x = [7, 3, 2, 0, 9, 6, 4, 5, 1, 8]
for t in x: mylist.add(t)
for t in x: print(mylist.rank(t), end=' ')
print("")

mylist = SetSkiplist()
x = [1,1,1,2,3,3]
for t in x: mylist.add(t)
for t in x: print(mylist.rank(t), end=' ')
    