#define N 100000
#define LEFT(x) (x << 1)
#define RIGHT(x) ((x << 1) | 1)

vector<pair<int, int> > g[N + 1];
bool seen[N + 1];
int parent[N + 1];
int weight[N + 1];
int size[N + 1];
int depth[N + 1];
int d[N + 1]; // Discovery time, which represents the position in the Segment Tree.
int d_inv[N + 1];
int seg[4 * N + 1];
int chain[N + 1]; // chain[u] stores the chain index of the vertex u. chain[u] = u if u is the representative of his chain.
int timer, n;

/* O(V + E) - Fills parent[], weight[], size[] and depth[]. */
void dfs_init(int u, int cur_depth){
	int v, w, i;

	seen[u] = true;
	size[u] = 1;
	depth[u] = cur_depth;

	for (i = 0; i < (int)g[u].size(); i++){
		v = g[u][i].first;
		w = g[u][i].second;

		if (!seen[v]){
			dfs_init(v, cur_depth + 1);
			parent[v] = u;
			weight[v] = w;
			size[u] += size[v];
		}
	}
}

/* O(V + E) - Fills d[], d_inv[] and chain[]. */
void dfs_hld(int u){
	int v, i;

	d[u] = ++timer;
	d_inv[d[u]] = u;

	for (i = 0; i < (int)g[u].size(); i++){
		v = g[u][i].first;
		chain[v] = (i == 0 ? chain[u] : v); // Creates a new chain for every i != 0.
		dfs_hld(v);
	}
}

/* O(1). */
int merge(int nl, int nr){
	return max(nl, nr);
}

/* O(N). */
void build(int cur, int l, int r){
	int u, m = (l + r) / 2;

	if (l == r){
		// Obtaining the vertex with discovery time equal to l.
		u = d_inv[l];
		seg[cur] = weight[u];
		return;
	}

	build(LEFT(cur), l, m);
	build(RIGHT(cur), m + 1, r);

	seg[cur] = merge(seg[LEFT(cur)], seg[RIGHT(cur)]);
}

/* O(Log(N)). Use update(1, 1, n, d[u], w) to update u with the value w. */
void update(int cur, int l, int r, int pos, int w){
	int m = (l + r) / 2;

	if (l > pos or r < pos){
		return;
	}

	if (l == r){
		seg[cur] = w;
		return;
	}

	update(LEFT(cur), l, m, pos, w);
	update(RIGHT(cur), m + 1, r, pos, w);

	seg[cur] = merge(seg[LEFT(cur)], seg[RIGHT(cur)]);
}

/* O(Log(N)). */
int query(int cur, int l, int r, int i, int j){
	int nl, nr, m = (l + r) / 2;

	if (l > j or r < i){
		return 0;
	}

	if (l >= i and r <= j){
		return seg[cur];
	}

	nl = query(LEFT(cur), l, m, i, j);
	nr = query(RIGHT(cur), m + 1, r, i, j);

	return merge(nl, nr);
}

/* O(V + E) - Builds the Heavy-Light structure. */
void init(int root){
	int u, i;

	// Fill parent and size.
	memset(seen, false, sizeof(seen));
	memset(size, 0, sizeof(size));
	parent[root] = 0;

	dfs_init(root, 0);

	// Clears graph.
	for (u = 1; u <= n; u++){
		g[u].clear();
	}

	// Transforms the undirected tree into a directed tree.
	for (u = 1; u <= n; u++){
		if (u != root){
			g[parent[u]].push_back(make_pair(u, weight[u]));
		}
	}

	// Place the child v with the greatest subtree first in the adjacency list of u.
	for (u = 1; u <= n; u++){
		for (i = 1; i < (int)g[u].size(); i++){
			if (size[g[u][i].first] > size[g[u][0].first]){
				swap(g[u][i], g[u][0]);
			}
		}
	}

	// Building chains.
	chain[root] = root;
	dfs_hld(root);

	// Building Segment Tree.
	build(1, 1, n);
}

/* O(Log^2(N)) - Queries the path between u and v. */
int solve(int u, int v){
	int ans = 0;

	// While u and v don't belong to the same chain.
	while (chain[u] != chain[v]){
		if (depth[chain[u]] > depth[chain[v]]){ // Going up u's chain.
			ans = merge(ans, query(1, 1, n, d[chain[u]], d[u]));
			u = parent[chain[u]];
		}
		else{ // Going up v's chain.
			ans = merge(ans, query(1, 1, n, d[chain[v]], d[v]));
			v = parent[chain[v]];
		}
	}

	// Remove "+1" and manually fill weight[] to make queries on vertices instead of queries on edges.
	ans = merge(ans, query(1, 1, n, min(d[u], d[v]) + 1, max(d[u], d[v])));

	return ans;
}