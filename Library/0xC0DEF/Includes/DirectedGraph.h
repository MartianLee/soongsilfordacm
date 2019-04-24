#pragma once
#include "Graph.h"

struct DirectedGraph: public Graph<int>{
	DirectedGraph(int n=0):Graph<int>(n) {}

	void add_edge(int s, int e, bool dir=true) {
		Graph::add_edge(s, e, 1, dir);
	}

	vector<int> topo_sort() {
		vector<int> in(n);
		forh(i, 0, n){
			for(auto &j:g[i])
				in[j.e]++;
		}
		queue<int> q;
		forh(i, 0, n)
			if (!in[i])
				q.push(i);
		vector<int> ret;
		while (q.size()) {
			auto cur = q.front();
			ret.push_back(cur);
			q.pop();
			for (auto &i : g[cur])
				if (--in[i.e] == 0)
					q.push(i.e);
		}
		reverse(ret.begin(), ret.end());
		return ret;
	}

	pair<vector<vector<int>>, DirectedGraph> scc(){
		vector<int> state(n), ord(n), scc_id(n, -1);
		stack<int> stk;
		int o=0, scc_cnt=0;
		forh(i, 0, n){
			if(!state[i])
				dfs_scc(i, o, scc_cnt, state, ord, scc_id, stk);
		}

		auto group=vector<vector<int>>(scc_cnt);
		forh(i, 0, n)
			group[scc_id[i]].push_back(i);
		DirectedGraph sccg(scc_cnt);
		forh(i, 0, n){
			for(auto& j:g[i])
				if(scc_id[i]!=scc_id[j.e])
					sccg.add_edge(scc_id[i], scc_id[j.e]);
		}

		return {group, sccg};
	}

private:
	int dfs_scc(int v, int& o, int &scc_cnt, vector<int>& state, vector<int>& ord, vector<int>& scc_id, stack<int>& stk){
		state[v]=1;
		stk.push(v);
		int ret = ord[v]=o++;
		for(auto i: g[v]){
			switch(state[i.e]){
				case 0:
					ret = min(ret, dfs_scc(i.e, o, scc_cnt, state, ord, scc_id, stk));
					break;
				case 1://back
					ret = min(ret, ord[i.e]);
					break;
				case 2:
					if(ord[i.e]>ord[v])//fwd
						;
					else if(ord[i.e]<ord[v]){//cross
						if(scc_id[i.e]==-1)
							ret = min(ret, ord[i.e]);
					}
					else//loop
						;
					break;
			}
		}
		if(ret == ord[v]){
			int cur;
			do{
				cur = stk.top();
				stk.pop();
				scc_id[cur]=scc_cnt;
			}while(stk.size() && cur!=v);
			scc_cnt++;
		}

		state[v]=2;
		return ret;
	}
};