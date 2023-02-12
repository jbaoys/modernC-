#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

// Complete the minimumBribes function below.
void minimumBribes(vector<int> q) {
    size_t n = q.size();
    int minBribes = 0;
    int comp = 0;
    int buf = 0;
    for (int i=0; i<n; ++i) {
        int dif = q[i] - i - 1;
        if (dif > 0) {
            if (dif < 3) {
                comp++;
                minBribes += dif;
                buf += dif;
            } else {
                cout << "Too chaotic\n";
                return;
            }
        } else {
            int dif2 = dif + comp;
            if (dif2 > 0) {
                comp++;
                minBribes++;
            }
            buf += dif;
            if (buf == 0) {
                comp = 0;
            } else if (buf < 0) {
                cout << "Too chaotic\n";
                return;
            }
        }
        cout << i+1 << ") buf=" << buf << ", " << "comp=" << comp << ", " << "minBribes=" << minBribes << endl;
    }
    cout << minBribes << endl;
}

int main()
{
    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) {
        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string q_temp_temp;
        getline(cin, q_temp_temp);

        vector<string> q_temp = split_string(q_temp_temp);

        vector<int> q(n);

        for (int i = 0; i < n; i++) {
            int q_item = stoi(q_temp[i]);

            q[i] = q_item;
        }

        minimumBribes(q);
    }

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

