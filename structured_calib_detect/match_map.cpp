#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <unordered_map>
#include <typeinfo>

using namespace std;

int main( int argc, char *argv[]) {

    ifstream direct, inverse;
    direct.open("direct_map.csv", ifstream::in);
    inverse.open("inverse_map.csv", ifstream::in);

    unordered_map<string, vector<int>> direct_map;
    unordered_map<string, vector<int>> inverse_map;

    // Read direct map
    {
        string tmp,s1,s2;
        int i,j;
        char c;
        while ( direct.peek()!=EOF ) {

            direct >> tmp;
            s1 = tmp.substr(0, tmp.find(","));
            s2 = tmp.substr(tmp.find(",")+1, tmp.size());
            i = stoi(s2.substr(0, s2.find(",")));
            j = stoi(s2.substr(s2.find(",")+1, s2.size())); 
            
            direct_map[s1].push_back(i);
            direct_map[s1].push_back(j);
        }
    }

    // Read inverse map
    {
        string tmp,s1,s2;
        int i,j;
        char c;
        while ( inverse.peek()!=EOF ) {

            inverse >> tmp;
            s1 = tmp.substr(0, tmp.find(","));
            s2 = tmp.substr(tmp.find(",")+1, tmp.size());
            i = stoi(s2.substr(0, s2.find(",")));
            j = stoi(s2.substr(s2.find(",")+1, s2.size())); 
            
            inverse_map[s1].push_back(i);
            inverse_map[s1].push_back(j);
        }
    }


    int counts[38];
    for(int i = 0;i<38;i++)
        counts[i] = 0;

    for (auto dir_el : direct_map) {
        for (auto inv_el : inverse_map) {
            auto x = (bitset<38>(dir_el.first)) ^ (bitset<38>(inv_el.first));
            counts[x.count()-1]++;
        }
    }
    for(int i = 0;i<38;i++){
        cout << i << " " << counts[i] << endl;
    }

    direct.close();
    inverse.close();

    return 0;
}
