#include"particles.hpp"
#include"game.hpp"
#include<iterator>

namespace game {

    // add new particle
    void ParticleEmiter::addParticle(int x, int y, int type, int dir) {
        Particle p{ x,y,type,dir };
        particles.push_back(p);
    }

    // set particle images
    void ParticleEmiter::setImages(const std::vector<int> &v) {
        if(!images.empty()) {
            images.erase(images.begin(), images.end());
        }
        std::copy(v.begin(), v.end(), std::back_inserter(images));
    }
}