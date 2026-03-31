#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 100005;
vector<int> graph[MAXN];
int color[MAXN];
bool visited[MAXN];
bool canBeInvited[MAXN];
int low[MAXN], disc[MAXN], parent[MAXN];
int timer_val = 0;
vector<pair<int,int>> bridges;

// Find all bridges using Tarjan's algorithm
void findBridges(int u) {
    visited[u] = true;
    disc[u] = low[u] = timer_val++;

    for (int v : graph[u]) {
        if (!visited[v]) {
            parent[v] = u;
            findBridges(v);
            low[u] = min(low[u], low[v]);

            // Check if edge u-v is a bridge
            if (low[v] > disc[u]) {
                bridges.push_back({min(u,v), max(u,v)});
            }
        } else if (v != parent[u]) {
            low[u] = min(low[u], disc[v]);
        }
    }
}

// BFS to check if a 2-edge-connected component is bipartite
bool checkBipartite(int start, vector<int>& component, vector<vector<int>>& bridgeFreeGraph) {
    queue<int> q;
    q.push(start);
    color[start] = 0;
    visited[start] = true;
    component.push_back(start);

    bool isBipartite = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : bridgeFreeGraph[u]) {
            if (!visited[v]) {
                visited[v] = true;
                color[v] = 1 - color[u];
                q.push(v);
                component.push_back(v);
            } else if (color[v] == color[u]) {
                isBipartite = false;
            }
        }
    }

    return isBipartite;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        graph[x].push_back(y);
        graph[y].push_back(x);
    }

    // Find all bridges
    memset(visited, false, sizeof(visited));
    memset(parent, -1, sizeof(parent));
    for (int i = 1; i <= n; i++) {
        if (!visited[i] && !graph[i].empty()) {
            findBridges(i);
        }
    }

    // Build graph without bridges
    vector<vector<int>> bridgeFreeGraph(n + 1);
    for (int u = 1; u <= n; u++) {
        for (int v : graph[u]) {
            pair<int,int> edge = {min(u,v), max(u,v)};
            if (find(bridges.begin(), bridges.end(), edge) == bridges.end()) {
                bridgeFreeGraph[u].push_back(v);
            }
        }
    }

    // Check each 2-edge-connected component
    memset(visited, false, sizeof(visited));
    memset(color, -1, sizeof(color));
    memset(canBeInvited, false, sizeof(canBeInvited));

    for (int i = 1; i <= n; i++) {
        if (!visited[i] && !bridgeFreeGraph[i].empty()) {
            vector<int> component;
            bool isBipartite = checkBipartite(i, component, bridgeFreeGraph);

            if (!isBipartite) {
                // Non-bipartite 2-edge-connected component
                for (int v : component) {
                    canBeInvited[v] = true;
                }
            }
        }
    }

    // Count vertices that cannot be invited
    int answer = 0;
    for (int i = 1; i <= n; i++) {
        if (!canBeInvited[i]) {
            answer++;
        }
    }

    cout << answer << endl;

    return 0;
}
