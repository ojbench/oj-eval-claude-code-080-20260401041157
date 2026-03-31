#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

const int MAXN = 100005;
vector<int> graph[MAXN];
int color[MAXN];
bool visited[MAXN];
bool canBeInvited[MAXN];

// BFS to check if component is bipartite and mark all vertices
bool checkBipartite(int start, vector<int>& component) {
    queue<int> q;
    q.push(start);
    color[start] = 0;
    visited[start] = true;
    component.push_back(start);

    bool isBipartite = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : graph[u]) {
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

// Check if a vertex is in a biconnected component containing an odd cycle
// For now, use a simple heuristic: degree >= 2 in a non-bipartite component
void markInvitable(vector<int>& component, bool isNonBipartite) {
    if (!isNonBipartite) {
        // Bipartite component - no one can be invited
        return;
    }

    // Non-bipartite component
    // Mark vertices with degree >= 2 as potentially invitable
    for (int v : component) {
        if (graph[v].size() >= 2) {
            // Need to check if v is actually in an odd cycle
            // For now, mark as potentially invitable
            canBeInvited[v] = true;
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
    memset(color, -1, sizeof(color));
    memset(canBeInvited, false, sizeof(canBeInvited));

    // Check each connected component
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            vector<int> component;
            bool isBipartite = checkBipartite(i, component);
            markInvitable(component, !isBipartite);
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
