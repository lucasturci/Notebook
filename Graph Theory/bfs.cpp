constexpr int N = 100000;

vector<int> g[N + 1]; // (Input)
int dist[N + 1];
bool seen[N + 1];

/* O(V + E). */
void bfs(int u) {
	queue<int> q;

	// Initializing.
	memset(seen, false, sizeof(seen));
	seen[u] = true;
	dist[u] = 0;
	q.push(u);

	// While there are vertices to visit.
	while (!q.empty()) {
		u = q.front();
		q.pop();

		// For every vertex v adjacent to u.
		for (int i = 0; i < g[u].size(); i++) {
			int v = g[u][i];

			// If vertex v wasn't already visited.
			if (!seen[v]) {
				seen[v] = true;
				dist[v] = dist[u] + 1;
				q.push(v);
			}
		}
	}
}