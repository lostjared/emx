#ifndef __STAR_FIELD_H__
#define __STAR_FIELD_H__

#include<iostream>
#include<string>
#include<vector>
#include<list>
#include"game.hpp"

namespace game {

    class Star {
    public:
        int x,y,type,dir;
    };

    class StarField {
    public:
        StarField();
        void init(int num_stars);
        void draw(RenderObject *ro);
        void proc();
        void setImages(const std::vector<Image> &i);
    protected:
        std::list<Star> stars;
        std::vector<Image> images;
    };
}

#endif