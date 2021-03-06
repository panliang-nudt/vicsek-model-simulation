#ifndef VICSEKQT_H_INCLUDED
#define VICSEKQT_H_INCLUDED

#include "Vicsek.h"
#include "QuadTree.h"

#include "SDL.h"
#include <SDL_image.h>

class VicsekQT : public Vicsek
{
private:


public:
    std::vector<Particle*> np;
    std::vector<Particle*> qt_np;

    QuadTree qt;

    // Constructor
    VicsekQT();

    // Constructor
    VicsekQT(SDL_Renderer* r, int width, int height, float v, float radius, float eta, unsigned int n_particles) : Vicsek(r, width, height, v, radius, eta, n_particles)
    {
        Rectangle rect = Rectangle(this->w/2.0, this->h/2.0, this->w, this->h);
        this->qt = QuadTree(NULL, rect,4,1);

        this->qt_np.reserve(this->n);
        this->np.reserve(this->n);
    }

    void createQuadTree();

    void hightlightNeighbours(int x, int y);

    void getNeighbours(int x, int y, std::vector<Particle*> &np);

    Step();

    Draw(SDL_Renderer* r);
};

#endif // VICSEKQT_H_INCLUDED
