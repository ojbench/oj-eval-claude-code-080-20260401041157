#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

const int MAXN = 100005;
vector<int> graph[MAXN];
int parent[MAXN];
int depth[MAXN];
bool visited[MAXN];
bool inOddCycle[MAXN];

void markCycle(int u, int v) {
    // Mark all vertices in the cycle from v to u
    int curr = u;
    while (curr != v) {
        inOddCycle[curr] = true;
        curr = parent[curr];
    }
    inOddCycle[v] = true;
}

void dfs(int u, int par, int d) {
    visited[u] = true;
    parent[u] = par;
    depth[u] = d;

    for (int v : graph[u]) {
        if (!visited[v]) {
            dfs(v, u, d + 1);
        } else if (v != par) {
            // Back edge found - check cycle length
            int cycleLength = depth[u] - depth[v] + 1;
            if (cycleLength % 2 == 1) {
                // Odd cycle found - mark all vertices in this cycle
                markCycle(u, v);
            }
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;

    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        graph[x].push_back(y);
        graph[y].push_back(x);
    }

    memset(visited, false, sizeof(visited));
    memset(inOddCycle, false, sizeof(inOddCycle));
    memset(parent, -1, sizeof(parent));

    // Run DFS from each unvisited vertex
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            dfs(i, -1, 0);
        }
    }

    int cannotBeInvited = 0;
    for (int i = 1; i <= n; i++) {
        if (!inOddCycle[i]) {
            cannotBeInvited++;
        }
    }

    cout << cannotBeInvited << endl;

    return 0;
}
