#ifndef _CAMERA_H__
#define _CAMERA_H__

#include<iostream>
#include<algorithm>

namespace game {

  class Camera {
    public:
        Camera();
        Camera(int w, int h, int mx, int my);
        Camera(const Camera &c);
        Camera(Camera &&c) = default;
        Camera &operator=(const Camera &c);
        Camera &operator=(Camera &&c) = default;
        void init(int w, int h, int mx, int my);
        void move(float delta, float dx, float dy);
        int getX() const;
        int getY() const;
        int getCamX() const;
        int getCamY() const;
        int getWidth() const;
        int getHeight() const;
        void cameraXY(int x, int y, int &xx, int &yy);
        void reset();        
        void set(const Camera &c);
protected:   
        int x,y,width,height,max_x,max_y,speed_x,speed_y;
    };

}

#endif