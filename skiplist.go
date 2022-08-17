package main

import (
	"fmt"
	"math"
	"math/rand"
	"errors"
)

type Lnode struct {
	key int
	levels []*Lnode
	ptrlen []int
}

type Skiplist struct {
	head *Lnode
}

func InitLnode(cur *Lnode, _key int) {
	cur.key = _key;
	h := 1
	for rand.Intn(2) == 0 {h++}
	for i := 0; i < h; i++ {cur.levels = append(cur.levels, nil)}
	for i := 0; i < h; i++ {cur.ptrlen = append(cur.ptrlen, 1)}
}

func InitHead(head *Lnode) {
	head.key = -math.MaxInt32
	max_h := int(math.Log2(math.MaxInt32) + 2)
	head.levels = []*Lnode{}
	for i := 0; i < max_h; i++ {head.levels = append(head.levels, nil)}
	head.ptrlen = []int{}
	for i := 0; i < max_h; i++ {head.ptrlen = append(head.ptrlen, 1)}
}

func Constructor() Skiplist {
	var sl Skiplist
	var head Lnode
	sl.head = &head
	InitHead(sl.head)
	return sl
}

func (sl *Skiplist) last_before(t int) (*Lnode, int) {
	cur := sl.head
	rank := 0
	for true {
		ok := false
		for i := len(cur.levels)-1; i >= 0; i-- {
			if cur.levels[i] != nil {
				if cur.levels[i].key < t {
					rank += cur.ptrlen[i]
					cur = cur.levels[i]
					ok = true
					break
				}
			}
		}
		if !ok {break}
 	}
	return cur, rank 
}

func (sl *Skiplist) path_to(t int) []*Lnode {
	cur := sl.head
	path := []*Lnode{}
	for true {
		path = append(path, cur)
		ok := false
		for i := len(cur.levels)-1; i >= 0; i-- {
			if cur.levels[i] != nil {
				if cur.levels[i].key < t {
					cur = cur.levels[i]
					ok = true
					break 
				}
			}
		}
		if !ok {break}
	}
	return path
}

func max(x, y int) int {
	if x > y {return x}
	return y
}

func (sl *Skiplist) Add(t int) {
	path := sl.path_to(t)
	var cur Lnode
	InitLnode(&cur, t)
	k := 0
	tot_ptrlen := 1
	for j := len(path)-1; j >= 0; j-- {
		if k>0 {tot_ptrlen += path[j].ptrlen[k-1]}
		for i := k; i < len(path[j].levels) && i < len(cur.levels); i++ {
			cur.ptrlen[i] = 1 + path[j].ptrlen[i] - tot_ptrlen
			path[j].ptrlen[i] = tot_ptrlen

			cur.levels[i] = path[j].levels[i]
			path[j].levels[i] = &cur
		}
		for i := max(k, len(cur.levels)); i < len(path[j].levels); i++ {
			path[j].ptrlen[i]++
		}
		k = len(path[j].levels) 
 	}
}

func (sl *Skiplist) Erase(t int) bool {
	path := sl.path_to(t)
	cur := path[len(path)-1].levels[0]
	if cur == nil {return false}
	if cur.key != t {return false}
	k := 0
	for j := len(path)-1; j >= 0; j-- {
		for i := k; i < len(path[j].levels) && i < len(cur.levels); i++ {
			path[j].levels[i] = cur.levels[i]
			path[j].ptrlen[i] += cur.ptrlen[i]-1
		}
		for i := max(k, len(cur.levels)); i < len(path[j].levels); i++ {
			path[j].ptrlen[i]--
		}
		k = len(path[j].levels)
 	}
	cur = nil
	return true
}

func (sl *Skiplist) rank(t int) int {
	cur, rank := sl.last_before(t)
	if cur.levels[0] != nil {
		if cur.levels[0].key == t {
			return rank
		}
	}
	return -1
}

func (sl *Skiplist) Search(t int) bool {
    return sl.rank(t) != -1
}

func (sl *Skiplist) how_many_smaller_than(t int) int {
	_, rank := sl.last_before(t)
	return rank
}

func (sl *Skiplist) key_with_rank(r int) (int, error) {
	if r<0 {return 0, errors.New("Rank cannot be negative")}
	cur := sl.head
	rank := 0
	for true {
		ok := false
		for i := len(cur.levels)-1; i >= 0; i-- {
			if cur.levels[i] != nil {
				if rank + cur.ptrlen[i] < r+1 {
					rank += cur.ptrlen[i]
					cur = cur.levels[i]
					ok = true
					break
				}
			}
		}
		if !ok {break}
	}
	if rank + cur.ptrlen[0] == r+1 && cur.levels[0] != nil {return cur.levels[0].key, nil}
	return 0, errors.New("There is no key with this rank") 
}

func main() {
	sl := Constructor()
	x := []int{1,2,3}
	for i := 0; i < len(x); i++ {
		sl.Add(x[i])
	}
	for i := 0; i < len(x); i++ {
		fmt.Print(sl.rank(x[i]))
		fmt.Print(" ")
	}
	fmt.Println("")
}
