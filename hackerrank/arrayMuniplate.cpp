#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

// Complete the arrayManipulation function below.
long arrayManipulation(int n, vector<vector<int>> queries) {
    /**
    The tricky thing is that it will take too much time if the "add" opertion is
    applied to each element in a range. The top solution on this site convert the
    range add operation as follows:
    1. Add only the start position to the array (one add only)
    2. Subtract the position with the same value (K) right after
       the end position. 
    3. After applied above 1 and 2 to all operations, go over the array from left
       all the way to the right to do sum and pick up the max. The fact is that:
       for each range with value (K), if the max value after "merge" include this
       range, the max should include the value K, so there should be a "sum" of
       all the selected elements in tha array. During the "sum", the value (K) is
       counted at the range left, and discounted rigth after the range (right), since
       in above step 2, a minus value (K) is used. For the ranges overlap, this procedure
       will keep accumulating all the overlapped values, and reset to an original value
       when it passing the ranges, since the add and subtract is a pair operation
       on the same value (K). In turn, we can go over all the peaks and pick the
       max.
       In another word, the smart of this method is to convert it into a stock curve
       graph. When entering a range, increase the stock value; when leaving the same
       range, decreasing the stock value.
     */
    vector<long> arr(n+2,0); // for 1-based index, + extra 1 space for decreasing
                             // when passing the last range (..n).
    for (auto &q : queries) {
        arr[q[0]] += q[2];      // Increase the value when entering this range
        arr[q[1]+1] -= q[2];    // Decrease the value when leaving this range
    }
    long max = 0;
    long x = 0;
    for (auto &e : arr) {
        // Go over the arr do sum, the proccess is like a dynamic stock curve
        // "x" is current stock value and it will go up and down by adding "e"
        x += e;
        if (max<x) max = x;
    }
    return max;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string nm_temp;
    getline(cin, nm_temp);

    vector<string> nm = split_string(nm_temp);

    int n = stoi(nm[0]);

    int m = stoi(nm[1]);

    vector<vector<int>> queries(m);
    for (int i = 0; i < m; i++) {
        queries[i].resize(3);

        for (int j = 0; j < 3; j++) {
            cin >> queries[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    long result = arrayManipulation(n, queries);

    fout << result << "\n";

    fout.close();

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}

