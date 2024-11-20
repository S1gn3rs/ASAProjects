#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#define ll long long int

using namespace std;

ll n, m;
vector<vector<ll>> cayley_table;

void find_string(vector<vector<vector<ll>>>& dp, vector<ll>& sequence, ll i, ll j, ll result){

    if (i == j) {
        cout << sequence[i];
        return;
    }

    ll split = dp[j][i][0];
    ll left_result = dp[j][i][1];
    ll right_result = dp[j][i][2];

    cout << "(";
    find_string(dp, sequence, i, split, left_result);
    cout << " ";
    find_string(dp, sequence, split + 1, j, right_result);
    cout << ")";
}

void parenthesis(vector<ll>& sequence, ll target){

    vector<vector<vector<ll>>> dp(m, vector<vector<ll>>(m));

    for (ll i=0;i<m;i++) {
        dp[i][i].push_back(sequence[i]);
    }

    for (ll size=2;size<=m;size++) {
        for (ll i=0;i<=m-size;i++) {
            ll end=i+size-1;

            for (ll a=end-1; a>=i; a--) {
                for (ll left : dp[i][a]) {
                    for (ll right : dp[a+1][end]) {
                        ll result = cayley_table[left][right];

                        if (find(dp[i][end].begin(), dp[i][end].end(), result) == dp[i][end].end()) {
                            dp[i][end].push_back(result);

                            dp[end][i] = {a, left, right};
                        }
                    }
                }
            }
        }
    }

    if (find(dp[0][m-1].begin(), dp[0][m-1].end(), target) != dp[0][m-1].end()){
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

    for (ll i=1;i<=n;i++) {
        for (ll j=1;j<=n;j++) {
            cin >> cayley_table[i][j];
        }
    }

    for (ll i=0;i<m;i++) {
        cin >> sequence[i];
    }

    cin >> target;

    parenthesis(sequence, target);

    return 0;
}
