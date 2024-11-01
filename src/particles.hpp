#ifndef __PARTICLES_H__
#define __PARTICLES_H__
#include<list>
#include<vector>

namespace game {


    struct RenderObject;

    class Particle {
    public:
        int x,y,type,dir;
        Particle() = default;
        Particle(int xx, int yy, int t, int d) : x{xx}, y{yy}, type{t}, dir{d} {}
    };

    class ParticleEmiter {
    public:
        ParticleEmiter() = default;
        void setImages(const std::vector<int> &v);
        void addParticle(int x, int y, int type, int dir);

        std::list<Particle> particles;
        protected:  
        std::vector<int> images;  
    };

}

#endif