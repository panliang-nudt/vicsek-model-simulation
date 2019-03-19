#include <iostream>
#include <cmath>
#include <vector>

#include <SDL.h>

#include "QuadTree.h"

// Destructor

QuadTree::~QuadTree()
{
    this->clear();
}

// Constructor

QuadTree::QuadTree()
{
    this->capacity = 4;
    this->splitted = false;
}

// clears QuadTree and frees allocated memory

void QuadTree::clear()
{

    if(this->splitted)
    {
        this->nw->clear();
        delete this->nw;
        this->ne->clear();
        delete this->ne;
        this->sw->clear();
        delete this->sw;
        this->se->clear();
        delete this->se;

        this->splitted = false;
    }

}

// QuadTree Constructor

QuadTree::QuadTree(Rectangle b, unsigned short cap, unsigned short mul)
{
    this->capacity = cap;
    this->mul = mul;
    this->splitted = false;
    this->p.reserve(cap);
    this->boundary = b;
}

// Draws the QuadTree nodes

void QuadTree::Draw(SDL_Renderer* r)
{
    SDL_SetRenderDrawColor( r, 0xFF, 0x00, 0x00, 0x00 );
    SDL_Rect rect;
    rect.x = this->boundary.x - (this->boundary.width / 2);
    rect.y = this->boundary.y - (this->boundary.height / 2);
    rect.w = this->boundary.getWidth()+1;
    rect.h = this->boundary.getHeight()+1;
    SDL_RenderDrawRect(r,&rect);
    if(this->splitted)
    {
        this->nw->Draw(r);
        this->ne->Draw(r);
        this->sw->Draw(r);
        this->se->Draw(r);
    }

}

// returns amount of Particles in this QuadTree

unsigned short QuadTree::size()
{
    unsigned short size = this->p.size();

    if(this->splitted)
    {
        size += this->nw->size();
        size += this->ne->size();
        size += this->sw->size();
        size += this->se->size();
    }


    return size;
}

// returns amount of nodes of this QuadTree

unsigned short QuadTree::nodes()
{
    unsigned short nodes = 1;

    if(this->splitted)
    {
        nodes += this->nw->nodes();
        nodes += this->ne->nodes();
        nodes += this->sw->nodes();
        nodes += this->se->nodes();
    }


    return nodes;
}

// returns the particles which are in a rectangular space

bool QuadTree::query(Rectangle& r, std::vector<Particle*>& rp)
{
    int elements = this->p.size();
    if(elements > 0)
    {
        if(this->boundary.intersects(r))
        {
            for(int i = 0; i<elements; i++)
            {
                rp.push_back((this->p[i]));
            }

            if(splitted)
            {
                this->nw->query(r,rp);
                this->ne->query(r,rp);
                this->sw->query(r,rp);
                this->se->query(r,rp);
            }

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

// inserts a Particle to this QuadTree

bool QuadTree::insertPoint(Particle* ip)
{

    if(!this->boundary.contains(ip->x, ip->y))
    {
        return false;
    }

    if(this->capacity > this->p.size())
    {
        this->p.emplace_back(ip);
        return true;
    }

    if(!splitted)
    {

        float new_width = this->boundary.getWidth() / 2;
        float new_height = this->boundary.getHeight() / 2;

        float center_nw_x = this->boundary.x - (this->boundary.getWidth() / 4);
        float center_nw_y = this->boundary.y - (this->boundary.getHeight() / 4);
        this->nw = new QuadTree(Rectangle(center_nw_x,center_nw_y,new_width,new_height), this->capacity*this->mul, this->mul);

        float center_ne_x = this->boundary.x + (this->boundary.getWidth() / 4);
        float center_ne_y = this->boundary.y - (this->boundary.getHeight() / 4);
        this->ne = new QuadTree(Rectangle(center_ne_x,center_ne_y,new_width,new_height), this->capacity*this->mul, this->mul);

        float center_sw_x = this->boundary.x - (this->boundary.getWidth() / 4);
        float center_sw_y = this->boundary.y + (this->boundary.getHeight() / 4);
        this->sw = new QuadTree(Rectangle(center_sw_x,center_sw_y,new_width,new_height), this->capacity*this->mul, this->mul);

        float center_se_x = this->boundary.x + (this->boundary.getWidth() / 4);
        float center_se_y = this->boundary.y + (this->boundary.getHeight() / 4);
        this->se = new QuadTree(Rectangle(center_se_x,center_se_y,new_width,new_height), this->capacity*this->mul, this->mul);

        this->splitted = true;
    }

    if(!this->nw->insertPoint(ip))
    {
        if(!this->ne->insertPoint(ip))
        {
            if(!this->sw->insertPoint(ip))
            {
                if(!this->se->insertPoint(ip))
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                return true;
            }
        }
        else
        {
            return true;
        }
    }
    else
    {
        return true;
    }
}

