#ifndef _GFX_DATA_H_
#define _GFX_DATA_H_

#include<iostream>
#include<fstream>
#include<vector>
#include"game.hpp"

namespace game {

    struct GfxImage {
        Image image;
    };

    class GfxData {
    public:
        GfxData() = default;
        bool open(const std::string &filename);
        bool load(RenderObject *ro, const Color &c);
        
        template<typename F>
        bool load(F f) {
            if(!file.is_open()) {
                return false;
            }
            while(!file.eof()) {
                uint32_t len;
                file.read(reinterpret_cast<char*>(&len), sizeof(len));
                if(file) {
                    char *tmp = new char [len+1];
                    file.read(reinterpret_cast<char*>(tmp), len);
                    tmp[len] = 0;
                    std::string filename;
                    filename = tmp;
                    delete [] tmp;
                    uint32_t index = 0, solid = 0, obj = 0, length = 0;
                    file.read(reinterpret_cast<char*>(&index), sizeof(uint32_t));
                    file.read(reinterpret_cast<char*>(&solid), sizeof(uint32_t));
                    file.read(reinterpret_cast<char*>(&obj), sizeof(uint32_t));
                    file.read(reinterpret_cast<char*>(&length), sizeof(uint32_t));
                    char *buffer = new char [ length + 1 ];
                    file.read(buffer, length);
                    f(obj, buffer, length);     
                    delete [] buffer;
                }
            }
           close();
           return true;
        }

        void close();
    protected:
        std::vector<GfxImage> gfx;
        std::fstream file;
    };

}


#endif