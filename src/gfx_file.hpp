#ifndef _GFX_FILE_H__
#define _GFX_FILE_H__


#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<cstdint>
#include<vector>

namespace game {


    struct GfxItem {
        uint32_t index;
        uint32_t solid;
        uint32_t obj;
        std::string filename;
    };

    class GfxTable {
    public:
        void addItem(const GfxItem &item);
        void addItem(uint32_t index, uint32_t solid, uint32_t obj, const std::string &filename);
        bool buildTable(const std::string &cfg);
        void clear();
        std::vector<GfxItem> table;
    };

    class GfxCompress {
    public:
        bool open(const std::string &file);
        bool compress(const GfxTable &table);
        void close();
    protected:
        std::fstream file;

        void write_string(const std::string &item);
    };

    class GfxExtract {
    public:
        bool open(const std::string &filename);
        bool extract(GfxTable &table, const std::string &directory);
        bool list(GfxTable &table);
    protected:
        std::fstream file;
    };
}


#endif