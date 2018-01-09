#define N 100000
#define L 20

int ancestor[N + 1][L + 1];
int parent[N + 1];
int depth[N + 1];

/* O(V * log(V)). */
void lca_init(){
	int i, j;

	// Initializing.
	memset(ancestor, -1, sizeof(ancestor));

	// Base case for Dynamic Programming.
	for (i = 1; i <= N; i++){
		ancestor[i][0] = parent[i];
	}

	// For each j.
	for (j = 1; j <= L; j++){
		// For each vertex i.
		for (i = 1; i <= N; i++){
			// If it has 2^(j - 1) ancestor.
			if (ancestor[i][j - 1] != -1){
				// 2^j ancestor of vertex i is the 2^(j - 1) ancestor of 2^(j - 1) ancestor of i.
				ancestor[i][j] = ancestor[ancestor[i][j - 1]][j - 1];
			}
		}
	}
}

/* O(log(V)). */
int lca(int u, int v){
	int i;

	// Making u the deepest vertex.
	if (depth[u] < depth[v]){
		swap(u, v);
	}

	// Getting u to the same depth as v by making "jumps" to its 2^i ancestors for each i.
	for (i = L; i >= 0; i--){
		// If jumping to u's 2^i ancestor doesn't make it pass vertex v's depth.
		if (depth[u] - (1 << i) >= depth[v]){
			u = ancestor[u][i];
		}
	}

	// If u and v are the same.
	if (u == v){
		return u;
	}

	// Making u and v "jump" up without making them go up to lca(u, v) or above.
	for (i = L; i >= 0; i--){
		// Only jump if the jump makes them go below lca(u, v).
		if (ancestor[u][i] != ancestor[v][i]){
			u = ancestor[u][i];
			v = ancestor[v][i];
		}
	}

	// At the end, u and v are one level deeper than lca(u, v).
	return parent[u];
}