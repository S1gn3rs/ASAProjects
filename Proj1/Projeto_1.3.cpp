#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#define ll long long int
#define IOS ios::sync_with_stdio(0);cin.tie(0);

using namespace std;

ll n, m;
vector<vector<ll>> cayley_table;
vector<vector<unordered_map<ll, pair<ll, ll>>>> dp;


/**
 * @brief Fills the Cayley table with input values.
 *
 * Reads and stores the input values for the Cayley table.
 */
void fill_cayley_table(){
    cayley_table.assign(n+1, vector<ll>(n+1, 0));

    for (ll i = 1; i <= n; i++){
        for (ll j = 1; j <= n; j++){
            cin >> cayley_table[i][j];
        }
    }
}


/**
 * @brief Constructs the expression with parentheses.
 *
 * Recursively constructs the expression by adding parentheses around the
 * subproblems.
 *
 * @param expression The string to which the expression is appended.
 * @param sequence The input sequence of elements.
 * @param line The starting index.
 * @param col The ending index.
 * @param result The target result to achieve.
 */
void put_parentheses(string& expression, vector<ll>& sequence, ll line, ll col,\
    ll result){

    if (line == col){
        expression += to_string(sequence[line]);
        return;
    }
    auto it_1 = dp[line][col].find(result);
    auto it_2 = dp[col][line].find(result);
    if (it_1 != dp[line][col].end()){
        ll split = it_1->second.first;
        ll left_result = it_2->second.first;
        ll right_result = it_2->second.second;

        expression += "(";
        put_parentheses(expression, sequence, line, split, left_result);
        expression += " ";
        put_parentheses(expression, sequence, split + 1, col, right_result);
        expression += ")";
    }
}


/**
 * @brief Computes combinations of elements between two indices.
 *
 * Iterates over possible splits between line and column, combining results
 * from left and right subproblems using the Cayley table. tores results in the
 * dynamic table.
 *
 * @param line The starting index.
 * @param col The ending index.
 */
void possible_combinations(ll line, ll col){

    for (ll c = col-1; c >= line; c--){

        for (pair< ll, pair<ll, ll>> left_entry : dp[line][c]){

            for (pair< ll, pair<ll, ll>> right_entry : dp[c+1][col]){

                ll resLeft = left_entry.first, resRight = right_entry.first;
                ll result = cayley_table[resLeft][resRight];

                if (dp[line][col].find(result) == dp[line][col].end()){
                    dp[line][col][result] = {c, -1};
                    dp[col][line][result] = {resLeft, resRight};
                }
                else if ((ll) dp[line][col].size() == n) break;
            }
        }
    }
    return;
}


/**
 * @brief Constructs the dynamic programming table for the given sequence.
 *
 * This function initializes the dynamic programming table and fills it
 * based on the possible combinations of elements in the sequence. The table
 * is used to store the results of subproblems to avoid redundant calculations.
 *
 * @param sequence The input sequence of elements for which the dynamic table
 * is constructed.
 */
void construct_dynamic_table(vector<ll>& sequence){

    dp.assign(m, vector<unordered_map<ll, pair<ll, ll>>>(m));
    //main diagonal, seqValue : {index, -1}
    for (ll i = 0; i < m; i++){
        dp[i][i][sequence[i]] = {i, -1};
    }
    for (ll size = 2; size <= m; size++){
        for (ll line = 0; line <= m - size; line++){
            ll col = line + size - 1;
            possible_combinations(line, col);
        }
    }
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

    construct_dynamic_table(sequence);

    if (dp[0][m-1].find(target) != dp[0][m-1].end()){
        string expression;
        put_parentheses(expression, sequence, 0, m-1, target);
        cout << "1\n" << expression << "\n";
    }
    else cout << "0\n";

    return 0;
}
