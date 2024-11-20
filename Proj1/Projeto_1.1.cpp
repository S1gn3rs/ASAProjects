#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#define ll long long int

using namespace std;

ll n, m;
vector<vector<ll>> cayley_table;

void parenthesis(int* sequence, ll target) {

    vector<vector<unordered_map<ll, string>>> dp(m, vector<unordered_map<ll, string>>(m));

    for(ll i=0;i<m;i++){ 
        dp[i][i][sequence[i]] = to_string(sequence[i]);
    }

    for (ll size=2;size<=m;size++){
        for (ll i=0;i<=m-size;i++){
            ll end=i+size-1;
            for (ll a=i;a<end;a++){
                for (auto parcel_l : dp[i][a]){
                    for (auto parcel_r : dp[a+1][end]){

                        ll result = cayley_table[parcel_l.first][parcel_r.first];
                        string equation = "(" + parcel_l.second + " " + parcel_r.second + ")"; 
                        dp[i][end][result] = equation;

                    }
                }
            }
        }
    }

    if (dp[0][m-1].find(target)!=dp[0][m-1].end()){
        cout << "1\n" << dp[0][m-1][target] << "\n";
    } else{
        cout << "0" << "\n";
    }

    return;
}

int main() {

    cin >> n >> m;

    ll target;
    cayley_table.assign(n+1, vector<ll>(n+1, 0));
    int sequence[m];
    
    for (ll i=0;i<n;i++){ 
        for (ll j=0;j<n;j++){ 
            cin >> cayley_table[i+1][j+1];
        }
    }

    for (ll i=0;i<m;i++){ 
        cin >> sequence[i];
    }
    
    cin >> target;
    
    parenthesis(sequence, target);
    
    return 0;
}
