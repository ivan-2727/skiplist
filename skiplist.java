import java.util.*;
import java.lang.*;

class Lnode {
	int key;
	Lnode[] levels;
	int[] ptrlen;
	Lnode(int _key) {
		key = _key;
		int h = 1;
		Random rand = new Random();
		while(rand.nextInt(2) == 0) h++;
		levels = new Lnode[h];
		Arrays.fill(levels, null);
		ptrlen = new int[h];
		Arrays.fill(ptrlen, 1);
	}
};

class SetSkiplist {
	Lnode head;
	SetSkiplist() {
		head = new Lnode(Integer.MIN_VALUE);
		int max_h = (int)Math.round(Math.log(Integer.MAX_VALUE) / Math.log(2)) + 2;
		head.levels = new Lnode[max_h];
		Arrays.fill(head.levels, null);
		head.ptrlen = new int[max_h];
		Arrays.fill(head.ptrlen, 1);
	}
	Map.Entry <Lnode, Integer> last_before(int t) {
		Lnode cur = head;
		int rank = 0;
		while(true) {
			Boolean ok = false;
			for (int i = cur.levels.length-1; i>=0; i--) {
				if (cur.levels[i] != null) if (cur.levels[i].key < t) {
					rank += cur.ptrlen[i];
					cur = cur.levels[i];
					ok = true;
					break;
				} 
			}
			if (!ok) break;
		}
		return new AbstractMap.SimpleImmutableEntry<>(cur, rank);
	}
	
	int rank(int t) {
		Map.Entry <Lnode, Integer> res = last_before(t);
		if (res.getKey().levels[0] != null) if (res.getKey().levels[0].key == t) return res.getValue();
		return -1;
	}
	
	int how_many_smaller_than(int t) {
		return last_before(t).getValue();
	}
	
	int key_with_rank(int r) throws Exception {
		if (r<0) throw new Exception("Rank cannot be negative");
		Lnode cur = head;
		int rank = 0;
		while (true) {
			Boolean ok = false;
			for (int i = cur.levels.length-1; i>=0; i--) {
				if (cur.levels[i] != null) if (rank + cur.ptrlen[i] < r+1) {
					rank += cur.ptrlen[i];
					cur = cur.levels[i];
					ok = true;
					break;
				} 
			}
			if (!ok) break; 
		}
		if (rank + cur.ptrlen[0] == r+1 && cur.levels[0] != null) return cur.levels[0].key; 
		throw new Exception("There is no key with this rank");
	}
	
	Boolean search(int t) {
		return rank(t) != -1; 
	}
	
	List<Lnode> path_to(int t) {
		Lnode cur = head;
		List<Lnode> path = new ArrayList<Lnode>();
		while(true) {
			path.add(cur);
			Boolean ok = false;
			for (int i = cur.levels.length-1; i>=0; i--) {
				if (cur.levels[i] != null) if (cur.levels[i].key < t) {
					cur = cur.levels[i];
					ok = true;
					break; 
				} 
			}
			if (!ok) break; 
		}
		return path; 
	}
	
	void add(int t) {
		List<Lnode> path = path_to(t);
		Lnode cur = new Lnode(t);
		int k = 0;
		int tot_ptrlen = 1;
		for (int j = path.size()-1; j >= 0; j--) {
			if (k>0) tot_ptrlen += path.get(j).ptrlen[k-1];
			for (int i = k; i < path.get(j).levels.length && i < cur.levels.length; i++) {
				cur.ptrlen[i] = 1 + path.get(j).ptrlen[i] - tot_ptrlen; 
				path.get(j).ptrlen[i] = tot_ptrlen; 
				
				cur.levels[i] = path.get(j).levels[i]; 
				path.get(j).levels[i] = cur;
			}
			for (int i = Math.max(k, (int)cur.levels.length); i < path.get(j).levels.length; i++) {
				path.get(j).ptrlen[i]++; 
			}
			k = path.get(j).levels.length; 
		}
	}
	
	Boolean erase(int t) {
		List<Lnode> path = path_to(t); 
		Lnode cur = path.get(path.size()-1).levels[0];
        if (cur == null) return false; 
		if (cur.key != t) return false;
		int k = 0; 
		for (int j = path.size()-1; j >= 0; j--) {
			for (int i = k; i < path.get(j).levels.length && i < cur.levels.length; i++) {
				path.get(j).levels[i] = cur.levels[i];
				path.get(j).ptrlen[i] += cur.ptrlen[i] - 1;
			}
			for (int i = Math.max(k, (int)cur.levels.length); i < path.get(j).levels.length; i++) {
				path.get(j).ptrlen[i]--; 
			}
			k = path.get(j).levels.length; 
		}
		cur = null;
        return true; 
	}

}

public class skiplist {
	public static void main(String[] args){
		SetSkiplist mylist = new SetSkiplist();
		int[] x = {7, 3, 2, 0, 9, 6, 4, 5, 1, 8};
		for (int i = 0; i < x.length; i++) {
			mylist.add(x[i]);
		}
		for (int i = 0; i < x.length; i++) {
			System.out.print(mylist.rank(x[i]));
			System.out.print(" ");
		}
		System.out.println("");
		int[] y = {1,1,1,2,3,3};
		mylist = new SetSkiplist();
		for (int i = 0; i < y.length; i++) {
			mylist.add(y[i]);
		}
		for (int i = 0; i < y.length; i++) {
			System.out.print(mylist.rank(y[i]));
			System.out.print(" ");
		}
	}
}
 