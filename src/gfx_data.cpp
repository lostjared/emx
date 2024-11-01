#include"gfx_data.hpp"

namespace game {
    // open GfxData object with filename to gfx file
    bool GfxData::open(const std::string &filename) {
        file.open(filename, std::ios::in | std::ios::binary);
        if(!file.is_open()) {
            std::cout << "game: Error could not open resource file: " << filename << "\n";
            return false;
        }
        uint32_t magic = 0;
        file.read(reinterpret_cast<char*>(&magic), sizeof(magic));
        if(magic != 0x421) {
            std::cerr << "game: Invalid resource file...\n";
            close();
            return false;
        }
        return true;   
    }

    // close gfxData object
    void GfxData::close() {
        file.close();
    }

    // after open load with render object / color
    bool GfxData::load(RenderObject *ro, const Color &c) {    
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
                Image img = ro->loadImage(buffer, length, c);
                gfx.push_back ( {img} );
                delete [] buffer;
            }
       }
       close();
       return true;
    }

}