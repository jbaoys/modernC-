#include <bits/stdc++.h>
using namespace std;

vector<uint32_t> getPrimes(uint32_t limit) {
    vector<uint32_t> res;
    if (limit<2) return res;
    vector<bool> allnum(limit+1, true);
    for (int i=2; i*i < limit; i++) {
        if (allnum[i]) {
            for (int j=i*i; j<=limit; j+=i) {
                allnum[j] = false;
            }
        }
    }
    for (int i=2; i<=limit; ++i) {
        if (allnum[i]) res.push_back(i);
    }
    return res;
}

int main(int argc, char* argv[]) {
    if (argc!=2) {
        cout << "Usage: printPrimes <limitNum>\n";
        exit(-1);
    }
    uint32_t limit = atoi(argv[1]);
    vector<uint32_t> primes = getPrimes(limit);
    int ct = 0;
    for (const auto p : primes) {
        cout << p << ", ";
        ct++;
        if (!(ct%10)) cout << endl;
    }
    cout << endl;
}
