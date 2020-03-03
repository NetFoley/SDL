#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (SDL_CreateWindowAndRenderer(620, 520, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    /*
    surface = IMG_Load("totue.png");*/
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        return 3;
    }

    int width = 50;
    int height = 50;
    SDL_Rect carrePos;
    carrePos.x = width;
    carrePos.y = height;
    carrePos.w = width;
    carrePos.h = height;

    surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 200, 130, 160));

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        return 3;
    }

    int vitesseX = 0;
    int vitesseY = 0;

    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
        else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            break;
        }

        int mouseX = 0;
        int mouseY = 0;
        int x = 0,y = 0;
        int posX = 0, posY = 0;
        SDL_GetWindowSize(window, &x, &y);
        SDL_GetWindowPosition(window, &posX, &posY);
        if (SDL_GetGlobalMouseState(&mouseX, &mouseY))
        {
            carrePos.x = mouseX - posX - carrePos.w/2;
            carrePos.y = mouseY - posY - carrePos.h/2;
            if(carrePos.x < 0)
                carrePos.x = 0;
            if(carrePos.y < 0)
                carrePos.y = 0;
            if(carrePos.x + carrePos.w > x)
                carrePos.x = x - carrePos.w;
            if(carrePos.y + carrePos.h > y)
                carrePos.y = y - carrePos.h;

        }

        Uint8 r = 0xFF - 0xFF/((float)(x+1)/(float)(carrePos.x+1));
        Uint8 g = 0xFF - 0x7F/((float)(x+1)/(float)(carrePos.x+1)) - 0X7F/((float)(y+1)/(float)(carrePos.y+1));
        Uint8 b = 0xFF - 0xFF/((float)(y+1)/(float)(carrePos.y+1));
        if(r < 0x00)
            r = 0x00;
        if(g < 0x00)
            g = 0x00;
        if(b < 0x00)
            b = 0x00;

        SDL_SetRenderDrawColor(renderer, r, g, b, 0);
        SDL_RenderClear(renderer);
        int colorB = 255/((float)(x+1)/(float)(carrePos.x+1));
        int colorG = 127/((float)(x+1)/(float)(carrePos.x+1)) + 127/((float)(y+1)/(float)(carrePos.y+1));
        int colorR = 255/((float)(y+1)/(float)(carrePos.y+1));
        if(colorB < 0)
            colorB = 0;
        if(colorG < 0)
            colorG = 0;
        if(colorR < 0)
            colorR = 0;

        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, colorB, colorG, colorR));
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
            return 3;
        }
        SDL_RenderCopy(renderer, texture, NULL, &carrePos);
        SDL_RenderPresent(renderer);
    }
    SDL_FreeSurface(surface);
    //SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
