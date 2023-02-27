#include <iostream>
#include <string>
#include "tgaimage.h"
#include "Model.h"
#include "Timer.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const int IMAGE_HEIGHT = 200;
const int IMAGE_WIDTH = 200;

const Vec3f LIGHT = Vec3f(0, 0, -1);
Model *model = nullptr;
std::string model_name;


//Bresenham��s Line Drawing Algorithm
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
void DrawLine(const Vec2i& v0, const Vec2i& v1, TGAImage& image, const TGAColor& color){
    DrawLine(v0.x, v0.y, v1.x, v1.y, image, color);
}
Vec3f CalcBaryCentric(Vec2i* pos, const Vec2i& P){
    Vec3f res = Vec3f(pos[2].x - pos[0].x, pos[1].x - pos[0].x, pos[0].x - P.x)
            ^ Vec3f (pos[2].y - pos[0].y, pos[1].y - pos[0].y, pos[0].y - P.y);
    if(std::abs(res.z) < 1) return Vec3f (-1, 1, 1);
    return Vec3f(1.f-(res.x+res.y)/res.z, res.y/res.z, res.x/res.z);
}
void DrawTriangle_Sweep(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
    if(t0.y == t1.y && t0.y == t2.y) return ;    // important!
    // sort the vertices, t0, t1, t2 lower?to?upper
    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);
    int total_height = t2.y-t0.y;

    for (int y=t0.y; y<=t1.y; y++) {
        int segment_height = t1.y-t0.y+1;
        float alpha = (float)(y-t0.y)/total_height;
        float beta  = (float)(y-t0.y)/segment_height; // be careful with divisions by zero
        Vec2i A = t0 + (t2-t0)*alpha;
        Vec2i B = t0 + (t1-t0)*beta;

        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
        }
    }

    for (int y=t1.y; y<=t2.y; y++) {
        int segment_height =  t2.y-t1.y+1;
        float alpha = (float)(y-t0.y)/total_height;
        float beta  = (float)(y-t1.y)/segment_height; // be careful with divisions by zero
        Vec2i A = t0 + (t2-t0)*alpha;
        Vec2i B = t1 + (t2-t1)*beta;
        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
        }
    }
}
void DrawTriangle_Barycentric(Vec2i * pos, TGAImage& image, const TGAColor& color){
    Vec2i bboxmin(image.get_width() - 1, image.get_height() - 1);
    Vec2i bboxmax(0, 0);
    for(int i = 0; i < 3; ++i){
        bboxmax.x = std::max(bboxmax.x, pos[i].x);
        bboxmax.y = std::max(bboxmax.y, pos[i].y);
        bboxmin.x = std::min(bboxmin.x, pos[i].x);
        bboxmin.y = std::min(bboxmin.y, pos[i].y);
    }
    Vec2i P;
    for(P.x = bboxmin.x; P.x <= bboxmax.x; ++P.x){
        for(P.y = bboxmin.y; P.y <= bboxmax.y; ++P.y){
            Vec3f u = CalcBaryCentric(pos, P);
            if(u.x < 0 || u.y < 0 || u.z < 0) continue;
            image.set(P.x, P.y, color);
        }
    }
}
void DrawTriangle(Vec2i* pos, TGAImage& image, const TGAColor& color){
    //DrawTriangle_Sweep(pos[0], pos[1], pos[2], image, color);
    DrawTriangle_Barycentric(pos, image, color);
}
void GetScreenPos(const Vec2f& inputP0, Vec2i& outputP0){
    outputP0.x = (inputP0.x+1.)*IMAGE_WIDTH/2.;
    outputP0.y = (inputP0.y+1.)*IMAGE_HEIGHT/2.;

}
int main(int argc, char** argv) {

    Timer timer(__func__);
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
        Vec3f face_pos[3];
        Vec2i screen_pos[3];
        for (int j=0; j<3; j++) {
            face_pos[j].x = model->vertex(model->face(i)[j]).x;
            face_pos[j].y = model->vertex(model->face(i)[j]).y;
            face_pos[j].z = model->vertex(model->face(i)[j]).z;
            GetScreenPos(Vec2f(face_pos[j].x, face_pos[j].y), screen_pos[j]);
        }

        Vec3f n = Vec3f(face_pos[2] - face_pos[0]) ^ (face_pos[1] - face_pos[0]);
        n.normalize();
        float intensity = n * LIGHT;
        if(intensity > 0)  // Back-face culling
            DrawTriangle(screen_pos, image, TGAColor(intensity * 255, intensity * 255,intensity * 255, 255));

    }

    image.flip_vertically();    // i want to have the origin at the left bottom corner of the image
    image.write_tga_file(std::string("../output/" + model_name + ".tga").c_str());

    return 0;
}
