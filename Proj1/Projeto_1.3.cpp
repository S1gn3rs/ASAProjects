#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#define ll long long int

using namespace std;

ll n, m;
vector<vector<ll>> cayley_table;

void find_string(vector<vector<unordered_map<ll, pair<ll, ll>>>>& dp, vector<ll>& sequence, ll i, ll j, ll result){
    if (i == j){
        cout << sequence[i];
        return;
    }

    auto it_1 = dp[i][j].find(result);
    auto it_2 = dp[j][i].find(result);
    if (it_1 != dp[i][j].end()){
        ll split = it_1->second.first;
        ll left_result = it_2->second.first;
        ll right_result = it_2->second.second;

        cout << "(";
        find_string(dp, sequence, i, split, left_result);
        cout << " ";
        find_string(dp, sequence, split + 1, j, right_result);
        cout << ")";
    }
}

void parenthesis(vector<ll>& sequence, ll target){

    vector<vector<unordered_map<ll, pair<ll, ll>>>> dp(m, vector<unordered_map<ll, pair<ll, ll>>>(m));

    for (ll i=0;i<m;i++){
        dp[i][i][sequence[i]] = {i, -1};
    }

    for (ll size=2; size<=m; size++){
        for (ll i=0; i<=m-size; i++){
            ll end=i+size-1;

            for (ll a=end-1; a>=i; a--){
                for (auto left_entry : dp[i][a]){
                    for (auto right_entry : dp[a+1][end]){
                        ll left = left_entry.first, right = right_entry.first;
                        ll result = cayley_table[left][right];

                        if (dp[i][end].find(result) == dp[i][end].end()){
                            dp[i][end][result] = {a, -1};
                            dp[end][i][result] = {left, right};
                        }
                    }
                }
            }
        }
    }

    if (dp[0][m-1].find(target) != dp[0][m-1].end()){
        cout << "1\n";
        find_string(dp, sequence, 0, m-1, target);
        cout << "\n";
    }
    else{
        cout << "0\n";
    }

    return;
}

int main() {
    cin >> n >> m;

    ll target;
    cayley_table.assign(n+1, vector<ll>(n+1, 0));
    vector<ll> sequence(m);

    for (ll i = 1; i <= n; i++){
        for (ll j = 1; j <= n; j++){
            cin >> cayley_table[i][j];
        }
    }

    for (ll i=0;i<m;i++){
        cin >> sequence[i];
    }

    cin >> target;

    parenthesis(sequence, target);

    return 0;
}
