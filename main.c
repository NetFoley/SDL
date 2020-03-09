#include "SDL.h"
#include "SDL_image.h"
#include "cube.h"
#include <stdio.h>
#include <math.h>

#define NBRCERCLE 55

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (SDL_CreateWindowAndRenderer(620, 620, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    int w,h;
    SDL_GetWindowSize(window, &w, &h);
    cube carre;
    INIT_cube(&carre, 50 , 50, renderer);

    circle circle[NBRCERCLE];

    int rayon = getDiagonalLenght(w,h);

    for(int i = 0; i < NBRCERCLE; i++)
    {
        INIT_circle(&circle[i], w/2, h/2, ((rayon*2)-(rayon*2)*i/NBRCERCLE)/4, renderer);
        assignRandomColors(&circle[i].r, &circle[i].g, &circle[i].b);
    }

    SDL_SetTextureBlendMode(renderer, SDL_BLENDMODE_ADD);
    while (1) {
        SDL_bool FC = SDL_FALSE;
        /*********************
            GET INPUTS
        *********************/
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
        else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            break;
        }
        else if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
            SDL_GetWindowSize(window, &w, &h);
            rayon = getDiagonalLenght(w,h);
            for(int i = 0; i < NBRCERCLE; i++)
            {
                INIT_circle(&circle[i], w/2, h/2, ((rayon*2)-(rayon*2)*i/NBRCERCLE)/4, renderer);
                assignRandomColors(&circle[i].r, &circle[i].g, &circle[i].b);
            }
        }
        else if(event.key.keysym.scancode == SDL_SCANCODE_F11)
        {
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        }

        /*********************
            MOUSE INPUTS
        *********************/
        int mouseX = 0;
        int mouseY = 0;
        if (SDL_GetGlobalMouseState(&mouseX, &mouseY))
        {
            int posX = 0, posY = 0;
            SDL_GetWindowPosition(window, &posX, &posY);
            carre.pos.x = mouseX - posX - carre.pos.w/2;
            carre.pos.y = mouseY - posY - carre.pos.h/2;
            stayInWindow(window, &carre.pos.x, &carre.pos.y, carre.pos.w, carre.pos.h);
        }

        //COULEURS FOND
        Uint8 r=0,g=0,b=0;
        assignColor(window, &r, &g, &b, carre.pos.x, carre.pos.y, 0);
        SDL_SetRenderDrawColor(renderer, r, g, b, 0);
        SDL_RenderClear(renderer);


        //COULEURS CUBE
        Uint8 colorB=0, colorG=0, colorR=0;
        assignColor(window, &colorR, &colorG, &colorB, carre.pos.x, carre.pos.y, 1);
        SDL_FillRect(carre.surface, NULL, SDL_MapRGB(carre.surface->format, colorB, colorG, colorR));
        carre.texture = SDL_CreateTextureFromSurface(renderer, carre.surface);

        if (!carre.texture) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
            return 3;
        }

        SDL_RenderCopy(renderer, carre.texture, NULL, &carre.pos);
        for(int i = 0; i < NBRCERCLE; i++)
        {
            if(circle[i].rayon > (rayon)/2)
            {
                circle[i].rayon = 0;
            }
            else
            {
                circle[i].rayon++;
            }

            SDL_SetRenderDrawColor(renderer, circle[i].r, circle[i].g, circle[i].b, 0);
            DrawCircle(renderer, window, &circle[i]);
        }

        SDL_RenderPresent(renderer);
    }
    SDL_FreeSurface(carre.surface);
    SDL_DestroyTexture(carre.texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
