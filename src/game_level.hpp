#ifndef _GAME_LEVEL_H_
#define _GAME_LEVEL_H_

#include"camera.hpp"
#include"level.hpp"
#include"game.hpp"
#include"object.hpp"
#include"star_field.hpp"
#include"gfx_data.hpp"
#include"particles.hpp"
#include<vector>

namespace game {
     class GameLevel : public GameObject {
    public:
        GameLevel() = default;
        void init(RenderObject *ro) override;
        void draw(RenderObject *ro) override;
        void release(RenderObject *ro) override;
        void keydown(char key) override;
        void keyup(char key) override;  
        void loadLevel(const std::string &filename, const std::string &gfx_file, const std::string &background);
        void nextLevel();
        void loadResources(const std::string &gfx_file);
        bool checkPoint(const Point &p);
        void resetLevel();
        void newGame();
        int hX() const;
        int hY() const;
        void procParticles();
        #ifdef FOR_WASM
        std::string assets_path = "/assets";
        #else
        std::string assets_path = "assets";
        #endif

    protected:    
        Level level;
        Camera cam;
        std::vector<Image> images;
        std::vector<CObject *> objects;
        std::vector<Image> object_images;
        std::vector<Image> g_img_r;
        std::vector<Image> g_img_l;
        Image bg_img;
        Font arial;
        int tsize;
        float delta;
        const int TILE_W=16;
        static const int WINDOW_SIZE_W = 1280/16;
        static const int WINDOW_SIZE_H = 720/16;
        static const int HALF_MAP_W = (1280/16)/2;
        static const int HALF_MAP_H = (720/16)/2;
        RenderObject *render_object;
        Hero hero;
        bool field_enabled = false;
        StarField field;
        ParticleEmiter emiter;
        std::vector<Image> stars;
        std::vector<Image> hero_images_right, hero_images_left;
        std::vector<Image> shot;
        int score, lives, items;
    };
}














#endif