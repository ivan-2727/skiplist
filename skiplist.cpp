#include <bits/stdc++.h>
using namespace std; 
using lli = long long int;

#include <chrono>
using namespace std::chrono;

class Lnode {
public:
	int key;
	vector <Lnode*> levels;
	vector <int> ptrlen;
	Lnode(int _key) {
		key = _key;
		int h = 1;
		while(rand()%2 == 0) h++;
		levels = vector <Lnode*> (h, nullptr);
		ptrlen = vector <int> (h, 1); 
	}
};

class SetSkiplist {
public:
	Lnode* head;
	SetSkiplist(int capacity = INT_MAX, int low_lim = -INT_MAX) {
		head = new Lnode(low_lim);
		int max_h = (int)log2(capacity) + 2;
		head->levels = vector <Lnode*> (max_h, nullptr); 
		head->ptrlen = vector <int> (max_h, 1); 
	}
	
	pair <Lnode*, int> last_before(int t) {
		Lnode* cur = head;
		int rank = 0; 
		while(true) {
			bool ok = false;
			for (int i = cur->levels.size()-1; i >= 0; i--) {
				if (cur->levels[i]) if (cur->levels[i]->key < t) {
					rank += cur->ptrlen[i];
					cur = cur->levels[i]; 
					ok = true; 
					break; 
				}
			}
			if (!ok) break;
		}
		return {cur, rank}; 
	}
	
	int rank(int t) {
		pair <Lnode*, int> res = last_before(t);
		if (res.first->levels[0]) if (res.first->levels[0]->key == t) return res.second;
		return -1; 
	}
	
	int how_many_smaller_than(int t) {
		return last_before(t).second;
	}
	
	int key_with_rank(int r) {
		if (r<0) throw invalid_argument("Rank cannot be negative");
		Lnode* cur = head;
		int rank = 0;
		while(true) {
			bool ok = false;
			for (int i = cur->levels.size()-1; i >= 0; i--) {
				if (cur->levels[i]) if (rank + cur->ptrlen[i] < r+1) {
					rank += cur->ptrlen[i];
					cur = cur->levels[i]; 
					ok = true; 
					break; 
				}
			}
			if (!ok) break;
		}
		if (rank + cur->ptrlen[0] == r+1 and cur->levels[0]) return cur->levels[0]->key; 
		throw invalid_argument("There is no key with this rank");
		return 0; 
	}
	
	bool search(int t) {
		return rank(t)!=-1;
	}
	
	vector <Lnode*> path_to(int t) {
		Lnode* cur = head;
		vector <Lnode*> path; 
		while(true) {
			path.push_back(cur); 
			bool ok = false;
			for (int i = cur->levels.size()-1; i >= 0; i--) {
				if (cur->levels[i]) if (cur->levels[i]->key < t) {
					cur = cur->levels[i]; 
					ok = true; 
					break; 
				}
			}
			if (!ok) break;
		}
		return path;
	}
	
	void add(int t) {
		vector <Lnode*> path = path_to(t); 
		Lnode* cur = new Lnode(t);
		int k = 0; 
		int tot_ptrlen = 1; 
		for (int j = path.size()-1; j >= 0; j--) {
			if (k>0) tot_ptrlen += path[j]->ptrlen[k-1];
			for (int i = k; i < path[j]->levels.size() and i < cur->levels.size(); i++) {
				cur->ptrlen[i] = 1 + path[j]->ptrlen[i] - tot_ptrlen; 
				path[j]->ptrlen[i] = tot_ptrlen; 
				
				cur->levels[i] = path[j]->levels[i]; 
				path[j]->levels[i] = cur;
			}
			for (int i = max(k, (int)cur->levels.size()); i < path[j]->levels.size(); i++) {
				path[j]->ptrlen[i]++; 
			}
			k = path[j]->levels.size(); 
		}
		 
	}
	
	bool erase(int t) {
		vector <Lnode*> path = path_to(t); 
		Lnode* cur = path.back()->levels[0];
        if (!cur) return false; 
		if (cur->key != t) return false;
		int k = 0; 
		for (int j = path.size()-1; j >= 0; j--) {
			for (int i = k; i < path[j]->levels.size() and i < cur->levels.size(); i++) {
				path[j]->levels[i] = cur->levels[i];
				path[j]->ptrlen[i] += cur->ptrlen[i] - 1;
			}
			for (int i = max(k, (int)cur->levels.size()); i < path[j]->levels.size(); i++) {
				path[j]->ptrlen[i]--; 
			}
			k = path[j]->levels.size(); 
		}
		delete cur;
        return true; 
	}

};

int main(void) {
	 
	for (int tot = 32; tot <= 2e17; tot*=2) {
		auto t1 = high_resolution_clock::now();
		SetSkiplist mylist = SetSkiplist();
		for (int i = 0; i < tot; i++) mylist.add(i);
		auto t2 = high_resolution_clock::now();
		for (int i = 0; i < 2*tot; i++) mylist.search(i);
		auto t3 = high_resolution_clock::now();
		set <int> x;
		for (int i = 0; i < tot; i++) x.insert(i);
		auto t4 = high_resolution_clock::now();
		for (int i = 0; i < 2*tot; i++) x.find(i);
		auto t5 = high_resolution_clock::now();
		vector <int> v(tot);
		for (int i = 0; i < tot; i++) v[i] = i;
		auto t6 = high_resolution_clock::now();
		for (int i = 0; i < tot; i++) {
			for (int j = 0; j < tot; j++) {
				if (v[j] == i) break; 
			}
		}
		auto t7 = high_resolution_clock::now();
		// printf("Make skiplist t2 - t1 = %d\n", (duration_cast<milliseconds>(t2 - t1)).count());
		// printf("Search in skiplist t3 - t2 = %d\n", (duration_cast<milliseconds>(t3 - t2)).count());
		// printf("Make treeset t4 - t3 = %d\n", (duration_cast<milliseconds>(t4 - t3)).count());
		// printf("Search in treeset t5 - t4 = %d\n", (duration_cast<milliseconds>(t5 - t4)).count());
		printf("%d\t%d\t%d\t%d\t%d\t%d\n", (int)(duration_cast<milliseconds>(t2 - t1)).count(), 
										   (int)(duration_cast<milliseconds>(t3 - t2)).count(),
										   (int)(duration_cast<milliseconds>(t4 - t3)).count(),
										   (int)(duration_cast<milliseconds>(t5 - t4)).count(),
										   (int)(duration_cast<milliseconds>(t6 - t5)).count(),
										   (int)(duration_cast<milliseconds>(t7 - t6)).count());
	}
	 
	
	
	return 0;
}