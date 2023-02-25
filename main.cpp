#include <iostream>
#include <string>
#include "tgaimage.h"
#include "Model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const int IMAGE_HEIGHT = 800;
const int IMAGE_WIDTH = 800;
Model *model = nullptr;
std::string model_name;


//Bresenham¡¯s Line Drawing Algorithm
void DrawLine(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color){
    bool isSteep = false;
    if(std::abs(x0 - x1) < std::abs(y0 - y1)){
        isSteep = true;
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if(x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dy = y1 - y0;
    int dx = x1 - x0;
    int derr = std::abs(dy) * 2;
    int y = y0;
    int err = 0;
    for(int x = x0; x <= x1; ++x){
        if(isSteep) image.set(y, x, color);
        else image.set(x, y, color);

        err += derr;
        if(err > dx){
            y += (dy > 0 ? 1 : -1);
            err -= 2 * dx;
        }
    }
}

int main(int argc, char** argv) {
    if(argc == 2){
        model = new Model(argv[1]);
        model_name = argv[1];
    }else{
        model = new Model("../model/african_head.obj");
        model_name = "african_head.obj";
    }

    model_name = model_name.substr(0, model_name.find('.'));

    TGAImage image(IMAGE_WIDTH, IMAGE_HEIGHT, TGAImage::RGB);

    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j=0; j<3; j++) {
            Vec3f v0 = model->vertex(face[j]);
            Vec3f v1 = model->vertex(face[(j+1)%3]);

            int x0 = (v0.x+1.)*IMAGE_WIDTH/2.;
            int y0 = (v0.y+1.)*IMAGE_HEIGHT/2.;
            int x1 = (v1.x+1.)*IMAGE_WIDTH/2.;
            int y1 = (v1.y+1.)*IMAGE_HEIGHT/2.;

            DrawLine(x0, y0, x1, y1, image, white);
        }
    }

    image.flip_vertically();    // i want to have the origin at the left bottom corner of the image
    image.write_tga_file(std::string("../output/" + model_name + ".tga").c_str());
    return 0;
}
