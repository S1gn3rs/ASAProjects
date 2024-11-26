#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

int n, m;
vector<vector<int>> cayley_table;

void find_string(string& expression, vector<vector<vector<int>>>& dp, vector<int>& sequence, int i, int j, int result){

    if (i == j) {
        expression += to_string(sequence[i]);
        return;
    }   

    int split = 0, left_result = 0, right_result = 0;

    for(int e = 0; e < (int)dp[i][j].size(); e++){
        if(dp[i][j][e] == result){
            split = dp[j][i][3 * e];
            left_result = dp[j][i][3 * e + 1];
            right_result = dp[j][i][3 * e + 2];
            break;
        }
    }

    expression += "(";
    find_string(expression, dp, sequence, i, split, left_result);
    expression += " ";
    find_string(expression, dp, sequence, split + 1, j, right_result);
    expression += ")";
}

void parenthesis(vector<int>& sequence, int target){

    vector<vector<vector<int>>> dp(m, vector<vector<int>>(m));

    for (int i=0;i<m;i++) {
        dp[i][i].push_back(sequence[i]);
    }

    for (int size=2;size<=m;size++) {
        for (int i=0;i<=m-size;i++) {
            int end=i+size-1;

            for (int a = end-1; a >= i; a--) {   
                bool found_all = false;
                for (int left : dp[i][a]) {
                    for (int right : dp[a+1][end]) {
                        int result = cayley_table[left][right];
                        if (find(dp[i][end].begin(), dp[i][end].end(), result) == dp[i][end].end()){
                            dp[i][end].push_back(result);
                            dp[end][i].push_back(a);
                            dp[end][i].push_back(left);
                            dp[end][i].push_back(right);
                        }
                        //cout << i << " " << end << " -> with result " << result << " : ";
                        //for(auto u : dp[i][end]) cout << u << " ";cout << "\n";
                        if ((int) dp[i][end].size() == n){
                            //cout << "Entrei \n";
                            found_all = true;
                            break;
                        }
                    }
                if(found_all) break;
                }
            }
        }
    }

    if (find(dp[0][m-1].begin(), dp[0][m-1].end(), target) != dp[0][m-1].end()){
        string expression;
        find_string(expression, dp, sequence, 0, m-1, target);
        cout << "1\n" << expression << "\n";
    } 
    else{
        cout << "0\n";
    }

    return;
}

int main() {
    cin >> n >> m;

    int target;
    cayley_table.assign(n+1, vector<int>(n+1, 0));
    vector<int> sequence(m);

    for (int i=1;i<=n;i++) {
        for (int j=1;j<=n;j++) {
            cin >> cayley_table[i][j];
        }
    }

    for (int i=0;i<m;i++) {
        cin >> sequence[i];
    }

    cin >> target;

    parenthesis(sequence, target);

    return 0;
}
