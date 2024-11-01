#ifndef _LEVEL_H__
#define _LEVEL_H__
#include<cstdint>
#include<iostream>
#include<string>
#include<vector>
#include"game.hpp"

namespace game {

    struct Tile {
        Tile() = default;
        Tile(const Tile &tile);
        Tile(uint8_t c, uint8_t s, uint8_t i);
        Tile(Tile &&tile) = default;
        Tile& operator=(const Tile &tile);
        Tile& operator=(Tile &&tile) = default;
        uint8_t color = 0;
        uint8_t solid = 0;
        uint32_t img = 0;
        uint8_t layers[3] = {0};
        void set(const Tile &tile);
        void setTile(const Tile &tile);
    };   

    bool atPoint(int x1, int y1, int w, int h, int &x, int &y);

    class Level {
    public:
        Level();
        ~Level();
        void setLevelName(const std::string &name);
        std::string getLevelName() const;
        bool loadLevelByName(const std::string &filename);
        bool saveLevelByName(const std::string &filename);
        bool loadLevel(const std::string &filename);
        bool loadLevelText(const std::string &filename);
        bool saveLevel(const std::string &filename);
        bool saveLevelText(const std::string &filename);
        int32_t width, height;
        void create(int32_t w, int32_t h, const Tile &init_tile);
        Tile *at(int32_t x, int32_t y);
        void output_map(std::ostream &out);
        bool checkTileSolid(int x, int y);
        bool checkSolid(const Point &p);
        bool check(const std::vector<Point> &p);
        bool checkType(const Point &p, int type);
        bool checkTypes(const std::vector<Point> &p, int type);
        bool checkRect(const Rect &r);
        bool checkRectForType(const Rect &r, int type);
        bool checkRectsForType(const std::vector<Rect> &v, int type);
   protected:
        Tile **tiles;
        bool resizeTiles(int32_t w, int32_t h);
        void releaseTiles();
        bool serialize(std::ostream &out);
        bool unserialize(std::istream &in);
        void serialize_text(std::ostream &out);
        bool unserialize_text(std::istream &in);
        std::string level_name = "Untitled Level";
    };

    bool PointInRect(const Point &p, const Rect &r);
}

#endif