#include <stdio.h>
#include <GL/gl.h>
#include <SDL/SDL.h>

int main() {
        SDL_Event event;
        SDL_Surface *surface;
        int x, y, texture;
        unsigned int vmem[320*200];

        SDL_Init(SDL_INIT_VIDEO);
        surface = SDL_SetVideoMode(320*3, 200*3, 32, SDL_OPENGL|SDL_GL_DOUBLEBUFFER|SDL_HWSURFACE);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glEnable(GL_TEXTURE_2D);
        while(event.type!=SDL_QUIT) {
                SDL_PollEvent(&event);
                for(y=0; y<200; y++)
                        for(x=0; x<320; x++)
                                vmem[x+y*320] = 65536*(rand()%256) + 256*(rand()%256) + rand()%256;
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(GL_TEXTURE_2D, 0, 4, 320, 200, 0, GL_RGBA, GL_UNSIGNED_BYTE, vmem);
                glBegin(GL_QUADS);
                glTexCoord2f(0, 0);
                glVertex2f(-1, 1);
                glTexCoord2f(1, 0);
                glVertex2f(1, 1);
                glTexCoord2f(1, 1);
                glVertex2f(1, -1);
                glTexCoord2f(0, 1);
                glVertex2f(-1, -1);
                glEnd();
                // SDL_Delay(500);
                SDL_GL_SwapBuffers();
        }
        SDL_Quit();
}