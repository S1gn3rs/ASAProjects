#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#define IOS ios::sync_with_stdio(0);cin.tie(0);

using namespace std;

int n, m;
vector<vector<int>> cayley_table;
vector<vector<unordered_map<int, pair<int, int>>>> dp;


/**
 * @brief Fiints the Cayley table with input values.
 *
 * Reads and stores the input values for the Cayley table.
 */
void fill_cayley_table(){
    cayley_table.assign(n+1, vector<int>(n+1, 0));

    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= n; j++){
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
void put_parentheses(string& expression, vector<int>& sequence, int line, int col,\
    int result){

    if (line == col){
        expression += to_string(sequence[line]);
        return;
    }
    auto it_1 = dp[line][col].find(result);
    auto it_2 = dp[col][line].find(result);
    if (it_1 != dp[line][col].end()){
        int split = it_1->second.first;
        int left_result = it_2->second.first;
        int right_result = it_2->second.second;

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
void possible_combinations(int line, int col){

    for (int c = col - 1; c >= line; c--){

        bool found_all = false;
        for (pair< int, pair<int, int>> left_entry : dp[line][c]){

            for (pair< int, pair<int, int>> right_entry : dp[c+1][col]){

                int resLeft = left_entry.first, resRight = right_entry.first;
                int result = cayley_table[resLeft][resRight];

                if (dp[line][col].find(result) == dp[line][col].end()){
                    dp[line][col][result] = {c, -1};
                    dp[col][line][result] = {resLeft, resRight};
                }
                
                if ((int) dp[line][col].size() == n){
                    found_all = true;
                    break;
                }
            }
            if(found_all) break;
        }
    }
    return;
}


/**
 * @brief Constructs the dynamic programming table for the given sequence.
 *
 * This function initializes the dynamic programming table and fiints it
 * based on the possible combinations of elements in the sequence. The table
 * is used to store the results of subproblems to avoid redundant calculations.
 *
 * @param sequence The input sequence of elements for which the dynamic table
 * is constructed.
 */
void construct_dynamic_table(vector<int>& sequence){

    dp.assign(m, vector<unordered_map<int, pair<int, int>>>(m));
    //main diagonal, seqValue : {index, -1}
    for (int i = 0; i < m; i++){
        dp[i][i][sequence[i]] = {i, -1};
    }
    for (int size = 2; size <= m; size++){
        for (int line = 0; line <= m - size; line++){
            int col = line + size - 1;
            possible_combinations(line, col);
        }
    }
    return;
}



int main() {
    IOS;
    cin >> n >> m;
    int target;
    vector<int> sequence(m);

    fill_cayley_table();

    for (int i=0;i<m;i++) cin >> sequence[i];
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
