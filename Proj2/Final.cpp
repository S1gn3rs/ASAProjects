#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <climits>
#include <utility>

#define IOS ios::sync_with_stdio(0);cin.tie(0);

using namespace std;

int n, m, l;

struct pair_hash {
    template <class T1, class T2>
    int operator()(const pair<T1, T2>& pair) const {
        return hash<T1>()(pair.first) ^ (hash<T2>()(pair.second) << 1);
    }
};

// Function to calculate metro connectivity
int metro_connectivity(vector<set<int>>& updated_lines) {

    // Graph of lines: connect lines that share stations
    vector<set<int>> line_graph(l + 1);

    for (int i = 1; i <= l; ++i) {
        if (updated_lines[i].empty()) continue;

        for (int j = i + 1; j <= l; ++j) {
            if (updated_lines[j].empty()) continue;

            for (int station : updated_lines[i]) {
                if (updated_lines[j].count(station)) {
                    line_graph[i].insert(j);
                    line_graph[j].insert(i);
                }
            }
        }
    }

    int mc = 0;
    for (int start = 1; start <= l; ++start) {
        if (updated_lines[start].empty()) continue;

        vector<int> min_changes(l + 1, -1);
        queue<pair<int, int>> q;    // {current_line, number_of_changes}

        min_changes[start] = 0;
        q.push({start, 0});

        while (!q.empty()) {
            pair<int, int> front = q.front();
            int line = front.first, changes = front.second;
            q.pop();

            for (int neighbor : line_graph[line]) {
                if (min_changes[neighbor] == -1 || min_changes[neighbor] > changes + 1) {
                    min_changes[neighbor] = changes + 1;
                    q.push({neighbor, changes + 1});
                }
            }
        }

        for (int i = 1; i <= l; ++i) {
            if (updated_lines[i].empty()) continue;
            if (min_changes[i] == -1) return -1;
            mc = max(mc, min_changes[i]);
        }
    }

    return mc;
}

int main() {

    IOS;

    cin >> n >> m >> l;

    
    set<pair<int,int>> total_edges;
    vector<int> line_edges(l + 1), stations(n + 1);
    unordered_map<pair<int,int>, vector<int>, pair_hash> edges;


    vector<set<int>> lines(l + 1), updated_lines(l + 1);


    // Read edges and associate them with lines
    for (int i = 0; i < m; ++i) {
        int x, y, line;
        cin >> x >> y >> line;

        line_edges[line]++; stations[x] = 1, stations[y] = 1;
        lines[line].insert(x);
        lines[line].insert(y);

        if(x > y) swap(x, y);
        edges[{x, y}].push_back(line);
        total_edges.insert({x, y});
    }

    // Edge case 1 (check if there are any isolated stations)
    for (int station = 1; station <= n; station++) {
        if (!stations[station]) {
            cout << -1 << "\n";
            return 0;
        }
    }

    // Edge case 2 (check if a single line can cover all stations)
    for(int line = 1; line <= l; line++) {
        if (line_edges[line] >= n - 1) {
            cout << 0 << "\n";
            return 0;
        }
    }


    
    for(pair<int,int> edge : total_edges) {
        int best_line = edges[edge][0];
        if((int) edges[edge].size() > 1) {
            int greater = -1;
            for(int line : edges[edge]) {
                if(line_edges[line] > greater) {
                    greater = line_edges[line];
                    best_line = line;
                }
            }
        }
        updated_lines[best_line].insert(edge.first);
        updated_lines[best_line].insert(edge.second);
    }



    // Calculate metro connectivity based on the updated graph
    int result = metro_connectivity(updated_lines);
    cout << result << "\n";

    return 0;
}
