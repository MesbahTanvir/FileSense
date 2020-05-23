#include <string>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <filesystem>
#include <vector>

using namespace std;
using namespace filesystem;


static size_t hash_from_path(const path &p)
{
    ifstream is {p.c_str(),
    ios::in | ios::binary};
    if (!is) { throw errno; }
    string s;
    is.seekg(0, ios::end);
    s.reserve(is.tellg());
    is.seekg(0, ios::beg);
    s.assign(istreambuf_iterator<char>{is}, {});
    return hash<string>{}(s);
}

vector < path > process_files(){
    vector < path> files;
    unordered_map < size_t, path > original;
    vector < path > duplicates;
    for (const auto & entry : directory_iterator(current_path())){
        auto path = entry.path();
        if(is_directory(path) == false) {
            auto hash_value = hash_from_path(path);
            if(original.count(hash_value)){
                duplicates.push_back(original[hash_value]);
            }
            original[hash_value] = path;
        }
    }
    for(auto d: duplicates){
        rename(current_path()/d.filename(), current_path()/"dup"/d.filename());
    }
    return files;
}

int main()
{
    process_files();
    return 0;
}