#include <bits/stdc++.h>

using namespace std;

std::vector<std::string> string_split(std::string s, const char delimiter)
{
    size_t start=0;
    size_t end=s.find_first_of(delimiter);

    std::vector<std::string> output;

    while (end <= std::string::npos)
    {
        output.emplace_back(s.substr(start, end-start));

        if (end == std::string::npos)
            break;

        start=end+1;
        end = s.find_first_of(delimiter, start);
    }

    return output;
}

string getFileName(string& name, unordered_set<string>& names){
    string filename = name;
    int i = 1;
    while (names.count(filename)) {
        filename = name + "("+to_string(i++)+")";
    }
    return filename;
}

int main() {
    unordered_set<string> fnames;
    int q;
    string qs;
    getline(cin, qs);
    q = stoi(qs);

    // Process each command
    for(int a0 = 0; a0 < q; a0++){
        // Read the first string denoting the command type
        string command, file1, file2;
        getline(cin, command);
        // Write additional code to read the command's file name(s) and process the command
        vector<string> cmds = string_split(command, ' ');
        //for (auto i:cmds) cout<< i;
        if (cmds[0]=="crt") {
            file1 = getFileName(cmds[1],fnames);
            ofstream output(file1.c_str());
            fnames.emplace(file1);
            cout << "+ " << file1 << endl;
        } else if (cmds[0]=="del") {
            fnames.erase(cmds[1]);
            remove(cmds[1].c_str());
            cout << "- " << cmds[1] << endl;
         } else if (cmds[0] == "rnm") {
            file2 = getFileName(cmds[2], fnames);
            rename(cmds[1].c_str(), file2.c_str());
            fnames.erase(cmds[1]);
            fnames.emplace(file2);
            cout << "r " << cmds[1] << " -> " << file2 << endl;
        }
        // Print the output string appropriate to the command
    }
    return 0;
}
