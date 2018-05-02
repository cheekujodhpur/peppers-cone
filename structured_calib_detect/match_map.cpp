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

    vector<string> direct_map_s;
    vector<string> inverse_map_s;
    vector<vector<int>> direct_map_pt;
    vector<vector<int>> inverse_map_pt;

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
            
            direct_map_s.push_back(s1);
            vector<int> tmp_pt;
            tmp_pt.push_back(i);
            tmp_pt.push_back(j);
            direct_map_pt.push_back(tmp_pt);

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
            
            inverse_map_s.push_back(s1);
            vector<int> tmp_pt;
            tmp_pt.push_back(i);
            tmp_pt.push_back(j);
            inverse_map_pt.push_back(tmp_pt);

        }
    }


    int counts[39];
    for(int i = 0;i<39;i++)
        counts[i] = 0;

    ofstream pixel_map;
    pixel_map.open("pixel_map.csv", ofstream::out);

    int tmp_count;

#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0;i<direct_map_s.size();i++) {
        for (int j = 0;j<inverse_map_s.size();j++) {
            auto x = (bitset<38>(direct_map_s[i])) ^ 
                     (bitset<38>(inverse_map_s[j]));

#pragma omp critical
            {
                tmp_count = x.count();
                if (tmp_count <= 5) {
                    pixel_map << tmp_count << ",";
                    pixel_map << direct_map_pt[i][0] << ",";
                    pixel_map << direct_map_pt[i][1] << ",";
                    pixel_map << inverse_map_pt[i][0] << ",";
                    pixel_map << inverse_map_pt[i][1] << "\n";
                }
                counts[tmp_count]++;
            }
        }
    }
    for(int i = 0;i<39;i++){
        cout << i << " " << counts[i] << endl;
    }

    pixel_map.close();
    direct.close();
    inverse.close();

    return 0;
}
