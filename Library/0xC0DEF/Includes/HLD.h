#pragma once
#include "LCA.h"

template<typename T, typename U=T, typename F=SegFDefault<T, U>>
struct HLD:public LCA<T>{
	using P=LCA<T>;
	HLD(const Tree<T>& t, int r)
	:P(t, r), st(n), chain(n), head(n), segidx(n), sz(n){
		dfs_size(r);
		int segi=0, cur_chain=0;
		dfs_hld(r, segi, cur_chain);
		head[0]=0;
	}

	//commutativeness free
	T query(int u, int v, bool edge_w=true){
		T ret = F::idT();
		int w = lca(u, v);
		while(chain[w]!=chain[u])
			ret = F::q(ret, st.query(segidx[head[chain[u]]], segidx[u]+1)), u = parent[head[chain[u]]].e;
		ret = F::q(ret, st.query(segidx[w]+edge_w, segidx[u]+1));

		vector<pair<int,int>> ranges;
		while(chain[w]!=chain[v])
			ranges.emplace_back(segidx[head[chain[v]]], segidx[v]+1), v = parent[head[chain[v]]].e;
		ranges.emplace_back(segidx[w]+1, segidx[v]+1);
		reverse(ranges.begin(), ranges.end());
		for(const auto&i:ranges)
			ret = F::q(ret, st.query(i.first, i.second));

		return ret;
	}

	void update(int cur, U w){return st.update(segidx[cur], w);}
	void update(int s, int e, U w){}
protected:
	using P::n;
	using P::r;
	using P::parent;
	using P::children;
	using P::lca;
	SegTree<T, U, F> st;
	vector<int> chain;
	vector<int> head;
	vector<int> segidx;
	vector<int> sz;

	int dfs_size(int cur){
		sz[cur]=1;
		for(const auto& i:children[cur])
			sz[cur]+=dfs_size(i.e);
		sort(children[cur].begin(), children[cur].end(), [&](auto a, auto b){return sz[a.e]>sz[b.e];});
		return sz[cur];
	}

	void dfs_hld(int cur, int& segi, int& cur_chain){
		st.update(segidx[cur]=segi++, parent[cur].w);
		chain[cur]=cur_chain;
		auto c = children[cur];
		forh(i, 0, c.size()){
			if(i)
				head[++cur_chain]=c[i].e;
			dfs_hld(c[i].e, segi, cur_chain);
		}
	}
};