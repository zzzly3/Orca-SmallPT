#include <bits/stdc++.h>
using namespace std;

int main(int argc,char *argv[])
{
    if (argc < 4 || argc % 2 == 1) {
        cerr << "Usages: mix OUTFILE [INFILE1 INSAMPLE1]..." << endl;
        return 1;
    }
    int w = 0, h = 0, s = 0;
    vector<uint64_t> p;
    for (int i = 2; i < argc; i+=2) {
        int sa = stoi(argv[i+1]);
        ifstream f(argv[i]);
        cout << argv[i] << " >> sample=" << sa << endl;
        string r;
        f >> r;
        if (r != "P3")
            return 2;
        int wi, he;
        f >> wi >> he;
        if (!s)
            w = wi, h = he, p.resize(w*h*3);
        else if (w != wi || h != he)
            return 3;
        int t;
        f >> t;
        if (t != 255)
            return 4;
        for (int j = 0; j < w*h*3; j++) {
            f >> t;
            p[j] += t * sa;
        }
        s += sa;
    }
    ofstream f(argv[1]);
    cout << argv[1] << " << sample=" << s << endl;
    f << "P3\n" << w << " " << h << "\n255\n";
    for (int i = 0; i < w*h*3; i++) {
        f << int(p[i] / s) << " ";
    }
    return 0;
}