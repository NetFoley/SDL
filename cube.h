#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED

#include <time.h>
#include <math.h>

typedef struct cube{
    SDL_Rect pos;
    SDL_Surface * surface;
    SDL_Texture * texture;

}cube;

typedef struct circle{
    SDL_Point centre;
    float rayon;
    SDL_Surface * surface;
    SDL_Texture * texture;
    Uint8 r;
    Uint8 g;
    Uint8 b;

}circle;

int INIT_cube(cube * cube, int width, int height, SDL_Renderer * renderer);
int INIT_circle(circle * circle, int centreX, int centreY, int rayon, SDL_Renderer * renderer);
void stayInWindow(SDL_Window * window, int * x, int * y, int weight, int height);
void assignColor(SDL_Window * window, Uint8 * r, Uint8 * g, Uint8 * b, int pointX, int pointY, int mode);
void DrawCircle(SDL_Renderer * renderer, SDL_Window * window, circle * circle);
void assignRandomColors(Uint8 * r, Uint8 * g, Uint8 * b);
void drawThePoint(SDL_Renderer * renderer, SDL_Window * window, int x, int y);

int INIT_cube(cube * cube, int width, int height, SDL_Renderer * renderer)
{
    cube->pos.x = width;
    cube->pos.y = height;
    cube->pos.w = width;
    cube->pos.h = height;

    cube->surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_FillRect(cube->surface, NULL, SDL_MapRGB(cube->surface->format, 200, 130, 160));

    cube->texture = SDL_CreateTextureFromSurface(renderer, cube->surface);
    if (!cube->texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        return 3;
    }
    return 0;
}

int INIT_circle(circle * circle, int centreX, int centreY, int rayon, SDL_Renderer * renderer)
{
    circle->centre.x = centreX;
    circle->centre.y = centreY;
    circle->rayon = rayon;

    circle->surface = SDL_CreateRGBSurface(0, rayon, rayon, 32, 0, 0, 0, 0);
    SDL_FillRect(circle->surface, NULL, SDL_MapRGB(circle->surface->format, 200, 130, 160));

    circle->texture = SDL_CreateTextureFromSurface(renderer, circle->surface);
    if (!circle->texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        return 3;
    }
    return 0;
}

void DrawCircle(SDL_Renderer * renderer, SDL_Window * window, circle * circle)
{
   const int32_t diameter = (circle->rayon * 2);

   int32_t x = (circle->rayon - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      drawThePoint(renderer, window, circle->centre.x +x, circle->centre.y -y);
      drawThePoint(renderer, window, circle->centre.x + x, circle->centre.y + y);
      drawThePoint(renderer, window, circle->centre.x - x, circle->centre.y - y);
      drawThePoint(renderer, window, circle->centre.x - x, circle->centre.y + y);
      drawThePoint(renderer, window, circle->centre.x + y, circle->centre.y - x);
      drawThePoint(renderer, window, circle->centre.x + y, circle->centre.y + x);
      drawThePoint(renderer, window, circle->centre.x - y, circle->centre.y - x);
      drawThePoint(renderer, window, circle->centre.x - y, circle->centre.y + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

void drawThePoint(SDL_Renderer * renderer, SDL_Window * window, int x, int y)
{
    int w,h;
    SDL_GetWindowSize(window, &w, &h);
    if(x > 0 && x < w && y > 0 && y < h)
        SDL_RenderDrawPoint(renderer, x, y);
}

void stayInWindow(SDL_Window * window, int * pointX, int * pointY, int width, int height)
{
    int x = 0, y = 0;
    int posX = 0, posY = 0;
    SDL_GetWindowSize(window, &x, &y);
    SDL_GetWindowPosition(window, &posX, &posY);

    if(*pointX < 0)
        *pointX = 0;
    if(*pointY < 0)
        *pointY = 0;
    if(*pointX + width > x)
        *pointX = x - width;
    if(*pointY + height > y)
        *pointY = y - height;
}

void assignColor(SDL_Window * window, Uint8 * r, Uint8 * g, Uint8 * b, int pointX, int pointY, int mode)
{
    int x,y;
    SDL_GetWindowSize(window, &x, &y);
    switch(mode)
    {
    case 0:
        *r = 0xFF - 0xFF/((float)(x+1)/(float)(pointX+1));
        *g = 0xFF - 0x7F/((float)(x+1)/(float)(pointX+1)) - 0X7F/((float)(y+1)/(float)(pointY+1));
        *b = 0xFF - 0xFF/((float)(y+1)/(float)(pointY+1));
        if(*r < 0x00)
            *r = 0x00;
        if(*g < 0x00)
            *g = 0x00;
        if(*b < 0x00)
            *b = 0x00;
    break;

    case 1:
        *b = 255/((float)(x+1)/(float)(pointX+1));
        *g = 127/((float)(x+1)/(float)(pointX+1)) + 127/((float)(y+1)/(float)(pointY+1));
        *r = 255/((float)(y+1)/(float)(pointY+1));
        if(*b < 0)
            *b = 0;
        if(*g < 0)
            *g = 0;
        if(*r < 0)
            *r = 0;
    break;
    }

}

void assignRandomColors(Uint8 * r, Uint8 * g, Uint8 * b)
{
    *r= rand() % 255;
    *g= rand() % 255;
    *b= rand() % 255;
}

int getDiagonalLenght(w, h)
{
    int diagonal = 0;
    diagonal = sqrt(w*w + h*h);

    return diagonal;
}

#endif // CUBE_H_INCLUDED
