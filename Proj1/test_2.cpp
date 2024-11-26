#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#define ll long long int
#define IOS ios::sync_with_stdio(0);cin.tie(0);

using namespace std;

ll n, m;
vector<vector<ll>> cayley_table;
vector<vector<unordered_map<ll, pair<ll, ll>>>> dp;

void fill_cayley_table(){
    cayley_table.assign(n+1, vector<ll>(n+1, 0));

    for (ll i = 1; i <= n; i++){
        for (ll j = 1; j <= n; j++){
            cin >> cayley_table[i][j];
        }
    }
}

void put_parentheses(string& expression, vector<ll>& sequence, ll line, ll col,\
    ll result){

    if (line == col){
        expression += to_string(sequence[line]);
        return;
    }

    ll split = dp[line][col][result].first;
    ll left_result = dp[col][line][result].first;
    ll right_result = dp[col][line][result].second;

    expression += "(";
    put_parentheses(expression, sequence, line, split, left_result);
    expression += " ";
    put_parentheses(expression, sequence, split + 1, col, right_result);
    expression += ")";
}

void possible_combinations(ll line, ll col){

    for (ll c = line; c < col; c++){

        for (pair<ll, pair<ll, ll>> left_entry : dp[line][c]){

            for (pair<ll, pair<ll, ll>> right_entry : dp[c+1][col]){

                ll left_result = left_entry.first, right_result = right_entry.first;
                ll result = cayley_table[left_result][right_result];

                if (dp[line][col].find(result) == dp[line][col].end()){
                    dp[line][col][result] = {c, -1};
                    dp[col][line][result] = {left_result, right_result};
                }
                if ((ll) dp[line][col].size() == n) break;
            }
        }
    }
    return;
}

void construct_dynamic_table(vector<ll>& sequence){

    auto start = chrono::high_resolution_clock::now(); // Start timing

    dp.assign(m, vector<unordered_map<ll, pair<ll, ll>>>(m));

    for (ll i = 0; i < m; i++){
        dp[i][i][sequence[i]] = {i, -1};
    }
    for (ll size = 2; size <= m; size++){
        for (ll line = 0; line <= m - size; line++){
            ll col = line + size - 1;
            possible_combinations(line, col);
        }
    }

    auto end = chrono::high_resolution_clock::now(); // Start timing

    // Calculate and display the duration in milliseconds
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Execution Time: " << duration.count() << " ms\n";
    return;
}

int main() {
    IOS;

    cin >> n >> m;
    ll target;
    vector<ll> sequence(m);

    fill_cayley_table();

    for (ll i=0;i<m;i++) cin >> sequence[i];
    cin >> target;

    auto start = chrono::high_resolution_clock::now(); // Start timing

    construct_dynamic_table(sequence);

    auto end = chrono::high_resolution_clock::now(); // End timing

    if (dp[0][m-1].find(target) != dp[0][m-1].end()){
        string expression;
        put_parentheses(expression, sequence, 0, m-1, target);
        cout << "1\n";
        cout << expression;
        cout << "\n";
    }
    else cout << "0\n";


    // Calculate and display the duration in milliseconds
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    //cout << "Execution Time: " << duration.count() << " ms\n";

    return 0;
}
