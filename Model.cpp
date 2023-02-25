//
// Created by ASUS on 2023/2/25.
//
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include "model.h"

using namespace std;

Model::Model(const string& filename) : vertices_(), faces_() {
    ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    string line;
    while (!in.eof()) {
        getline(in, line);
        istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) iss >> v.raw[i];
            vertices_.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            faces_.push_back(f);
        }
    }
    std::cout <<"Load model success: ";
    std::cout << "v# " << vertices_.size() << " f# " << faces_.size() << std::endl;
}

Model::~Model() {}

int Model::nvertices() {
    return (int)vertices_.size();
}
int Model::nfaces() {
    return (int)faces_.size();
}
vector<int> Model::face(int idx) {
    return faces_[idx];
}
Vec3f Model::vertex(int idx) {
    return vertices_[idx];
}