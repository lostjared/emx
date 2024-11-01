#ifndef _OBJECT_H__
#define _OBJECT_H__

#include"game.hpp"
#include<vector>

namespace game {

    class Camera;
    class Level;

    enum class Direction { LEFT=1, RIGHT, UP, DOWN };

    class CObject {
    public:
        CObject() = default;
        virtual ~CObject() = default;
        virtual void init(RenderObject *ro) = 0;
        virtual void draw(RenderObject *ro, int x, int y) = 0;
        void setImages(const std::vector<Image> &v);
        virtual void release() = 0;
        virtual void logic(Level *level) = 0;
        virtual void reset() = 0;
        virtual void death() = 0;
   
        int x = 0,y = 0,cur_frame = 0, type = 0;
        std::vector<Image> frame_data;
        bool active = true;
        Direction dir;
        int otype = 0;
    };


    class Item : public CObject {
    public:
        Item() = default;
        Item(int xx, int yy, int type, int item_t, int item_a);
        void init(RenderObject *ro) override;
        void draw(RenderObject *ro, int x, int y) override;
        void release() override;
        void logic(Level *level) override;   
        void reset() override;
        void death() override;
        int grab();
        int item_type = 0;
        int item_amount = 0;
    };


    class Enemy : public CObject {
    public:
        Enemy() = default;
        Enemy(int x, int y, int type);
        void init(RenderObject *ro) override;
        void draw(RenderObject *ro, int x, int y) override;
        void release() override; 
        void setImages(const std::vector<Image> &l, const std::vector<Image> &r); 
        void reset() override;
        void logic(Level *level) override;
        void death() override;
    protected:
        std::vector<Image> g_img_l, g_img_r;
        int start_x, start_y;
        int draw_x, draw_y;
        int cur_ani;
        bool die = false;
    };

    class Hero : public CObject {
    public:
        Hero() = default;
        void init(RenderObject *ro) override;
        void setImages(const std::vector<Image> &vl, const std::vector<Image> &vr);
        void draw(RenderObject *ro, int x, int y) override;
        void release() override;
        void logic(Level *level) override;
        void death() override;
        void reset() override;
        void moveLeft(bool scroll);
        void moveRight(bool scroll);
        void moveDown(bool draw);
        void moveUp(bool scroll, Camera *cam);
        void restore();
        void cycle_frame();
        void jump();
        void fire();
        void proc_jump(Level *level, Camera *cam);
        bool isJumping() const;
        void update(Camera *cam);
        void updateDown(Level *level, Camera *cam);
        void set();
        bool grounded = false;
        bool falling = false;  
        int draw_x = 0, draw_y = 0;
        std::vector<Image> left, right;
        bool move_right = true;
        bool jumping = false;
        int jump_index = 0, jump_height[2] ={0};
        bool moving_ = false;
        int moving_index[4] = {0};
        bool scroll_map[5] =  {false}; 
        int walk_frames[8] = { 0, 1, 2, 3, 2, 1, 0 }; 
        int walk_frame = 0;
        bool shot = false;
    };

}


#endif