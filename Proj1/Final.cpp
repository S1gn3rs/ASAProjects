#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#define IOS ios::sync_with_stdio(0);cin.tie(0);

using namespace std;

int n, m;
vector<vector<int>> cayley_table;
vector<vector<vector<int>>> dp, found;


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
void put_parentheses(string& expression, vector<int>& sequence, int line,\
    int col, int result){

    if (line == col) {
        expression += to_string(sequence[line]);
        return;
    }

    int split = 0, left_result = 0, right_result = 0;

    for(int e = 0; e < (int)dp[line][col].size(); e++){
        if(dp[line][col][e] == result){
            split = dp[col][line][3 * e];
            left_result = dp[col][line][3 * e + 1];
            right_result = dp[col][line][3 * e + 2];
            break;
        }
    }

    expression += "(";
    put_parentheses(expression, sequence, line, split, left_result);
    expression += " ";
    put_parentheses(expression, sequence, split + 1, col, right_result);
    expression += ")";
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

    for (int c = col - 1; c >= line; c--) {
        bool found_all = false;
        for (int left_entry : dp[line][c]) {
            for (int right_entry : dp[c + 1][col]) {
                int result = cayley_table[left_entry][right_entry];
                if (found[line][col][result] == 0){
                    found[line][col][result] = 1;
                    dp[line][col].push_back(result);
                    dp[col][line].push_back(c);
                    dp[col][line].push_back(left_entry);
                    dp[col][line].push_back(right_entry);
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
 * This function initializes the dynamic programming table and fills it
 * based on the possible combinations of elements in the sequence. The table
 * is used to store the results of subproblems to avoid redundant calculations.
 *
 * @param sequence The input sequence of elements for which the dynamic table
 * is constructed.
 */
void construct_dynamic_table(vector<int>& sequence){

    dp.assign(m, vector<vector<int>>(m));
    found.assign(m, vector<vector<int>>(m, vector<int>(n + 1, 0)));

    for (int i=0;i<m;i++) {
        dp[i][i].push_back(sequence[i]);
        found[i][i][sequence[i]] = 1;
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

    if (found[0][m-1][target]){
        string expression;
        put_parentheses(expression, sequence, 0, m-1, target);
        cout << "1\n" << expression << "\n";
    }
    else cout << "0\n";

    return 0;
}