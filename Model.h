//
// Created by ASUS on 2023/2/25.
//

#pragma once

#pragma once
#include "geometry.h"
#include<vector>
#include<string>


class Model {
private:
    std::vector<Vec3f> vertices_;
    std::vector<std::vector<int>> faces_;
public:
    Model(const std::string& filename);
    ~Model();
    int nvertices();
    int nfaces();
    Vec3f vertex(int idx);
    std::vector<int> face(int idx);

};
