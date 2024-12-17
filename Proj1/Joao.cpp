#include <cstdio>
#include <iostream>
#include <vector>
#include <set>
#include <string>

using namespace std;

vector<int> _initSequence;
vector<vector<int>> _opTable;
string aux;

/* 
*   _subSeqData[i][j][k] = subsequence[i][j] result(s)
*   _subSeqData[i][j][k+1] = subsequence[i][j] leftvalue that made the result
*   _subSeqData[i][j][k+2] = subsequence[i][j] right value that made the result
*   _subSeqData[i][j][k+3] = subsequence[i][j] splitpoint that made the result
*
*
*/
vector<vector<vector<int>>> _subSeqData;
vector<vector<vector<int>>> _confirmation;

void parenthesization(int desiredVal, int start, int end) {

    if(start == end) {

        aux.append(to_string(_initSequence[start]));
        return;
    }

    for(int h = 0; h < static_cast<int>(_subSeqData[start][end].size()); h += 4) {

        if (_subSeqData[start][end][h] == desiredVal) {

            //cout << "found desired for \tdesired|\tstart|\t|end\n"<<desiredVal<<"\t"<<start<<"\t"<<end<<"\n";
            int _leftVal = _subSeqData[start][end][h + 1];
            int _rightVal = _subSeqData[start][end][h + 2];
            int _splitVal =  _subSeqData[start][end][h + 3];
            aux.append("(");
            parenthesization(_leftVal, start, _splitVal);
            aux.append(" ");
            parenthesization(_rightVal, _splitVal + 1, end);
            aux.append(")");
            return;
        }
    }
}


int main() {

    ios::sync_with_stdio(0);
    std::cin.tie(0);

    /* -----------------------input reading -----------------------*/
    
    int _opTableDim, _initSeqLen;
    if (scanf("%d %d", &_opTableDim, &_initSeqLen)) {}

    _opTable.resize(_opTableDim, vector<int>(_opTableDim, 0));
    for (int _l = 0; _l < _opTableDim; _l++) {

        for (int _c = 0; _c < _opTableDim; _c++) {

            if(scanf("%d", &_opTable[_l][_c])) {}
        }
    }

    _initSequence.resize(_initSeqLen);
    for (int _n = 0; _n < _initSeqLen; _n++) {

        if(scanf("%d", &_initSequence[_n])) {}
    }

    int _initSeqRes;
    if (scanf("%d", &_initSeqRes)) {}

    /* -----------------------input reading -----------------------*/



    /* -----------------------edge/base cases -----------------------*/

    if (_initSeqLen == 1) {

        if(_initSeqRes == _initSequence[0]) {
            
            aux.append("1\n" + to_string(_initSequence[0]) + "\n");
            cout << aux;
            return 0;
        }
        else {

            aux.append("0\n");
            cout << aux;
            return 0;
        }
    }
    /* -----------------------edge/base cases -----------------------*/

    _subSeqData.assign(_initSeqLen, vector<vector<int>>(_initSeqLen));
    _confirmation.assign(_initSeqLen, vector<vector<int>>(_initSeqLen, vector<int>(_opTableDim + 1, 0)));
    /* 
    *   Initializing matrix diagonal (a).
    */
    for (int i = 0; i < _initSeqLen; i++) {

        _subSeqData[i][i].push_back(_initSequence[i]);
        _confirmation[i][i][_initSequence[i]] = 1;
    }
    /* 
    *   Initializing matrix for (a+b).
    */

    for (int _subSeqLen = 2; _subSeqLen <= _initSeqLen; _subSeqLen++) {

        for (int _start = 0; _start <= _initSeqLen - _subSeqLen; _start++) {

            int _end = _start + _subSeqLen - 1;
            for (int _split = _end - 1; _split >= _start; _split--) {

                //cout << "------------\tSPLIT\t--------" << _split << "\n";
                int count = 0;
                const auto& leftValues = _subSeqData[_start][_split];
                const auto& rightValues = _subSeqData[_split + 1][_end];
                bool _everycombination = false;

                
                for(int _l = 0; _l < (int) leftValues.size(); _l += 4) {
                    for(int _r = 0; _r < (int) rightValues.size(); _r += 4) {
                        
                        int _leftVal = leftValues[_l];
                        int _rightVal = rightValues[_r];
                        int _binaryVal = _opTable[_leftVal - 1][_rightVal - 1];
                        if(!_confirmation[_start][_end][_binaryVal]){
                            count++;
                            _subSeqData[_start][_end].push_back(_binaryVal);
                            _subSeqData[_start][_end].push_back(_leftVal);
                            _subSeqData[_start][_end].push_back(_rightVal);
                            _subSeqData[_start][_end].push_back(_split);
                            _confirmation[_start][_end][_binaryVal] = 1;
                        }
                        if ((int) _subSeqData[_start][_end].size() == 4*_opTableDim) {
                            _everycombination = true;
                            break;
                        }
                        //cout << "\nleft\t|right|\t|result\t|split\n" << _leftVal << "\t" << _rightVal << "\t" << _binaryVal <<"\t" <<_split<< "\n";
                        
                    }
                    //cout <<"size:\t" << static_cast<int>(_resultValues.size()) << "\toptabledm:\t" << _opTableDim<<"\n";
                    if (_everycombination){

                        //cout << "found all results for split: " << _split << "\n";
                        break;
                    }
                }
            }
        }
    }

    for(int k = 0; k < (int) _subSeqData[0][_initSeqLen - 1].size(); k += 4) {

        if (_subSeqData[0][_initSeqLen - 1][k] == _initSeqRes) {

            //cout <<"found it\n";
            aux.append("1\n");
            parenthesization(_initSeqRes, 0, _initSeqLen - 1);
            cout << aux << "\n";
            return 0;
        }
    }

    aux.append("0\n");
    cout << aux;
    return 0;
}