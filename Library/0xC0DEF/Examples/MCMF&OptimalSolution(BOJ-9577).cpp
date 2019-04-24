#include "FlowGraph.h"

int main() {
	ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	cout << fixed << setprecision(10);
	srand((u32)time(0));

	int t;
	cin >> t;
	while (t--) {
		int n, m;
		cin >> n >> m;
		FlowGraph g(200);
		forh(i, 0, m)
		{
			int s, e, a;
			cin >> s >> e>>a;
			forh(j, 0, a)
			{
				int q;
				cin >> q;
				q--;
				forh(k, s, e)
					g.add_edge(k, 100 + q, 1, k*k*k);
			}
		}
		forh(i, 0, 100)
		{
			g.add_edge(g.src, i, 1, 0);
			g.add_edge(100 + i, g.snk, 1, 0);
		}

		if (g.mcmf().y != n)
			cout << -1 << endl;
		else
		{
			int ans = 0;
			forh(i, 0, 100)
				for (auto j : g.g[i])
					if (j.e < 200 && !j.w.cap)
						ans = i + 1;
			cout << ans << endl;
		}
	}

	return 0;
}