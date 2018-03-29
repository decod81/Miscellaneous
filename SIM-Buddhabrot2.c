// compile with -lSDL2main -lSDL2
// ffmpeg -f image2 -r 30 -s 1280x720 -pix_fmt gray -vcodec rawvideo -i %05d.raw -pix_fmt yuv420p -vcodec libx264 -crf 2 output.mp4

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int WinMain(int argc, char* args[]) {
	SDL_Event event;
	SDL_Window *window;
	SDL_Surface *surface;
	int X = 1280, Y = 720;
	int i, *p, x, y, n, c = 1;
	long long counter=0;
	float zr, zi, x0, y0, xt, yt;
	unsigned char *plane = malloc(X*Y);
	char filename[128];
	FILE *f;

	for(n=0; n<X*Y; n++)
		plane[n] = 0;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("SDL2Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, X, Y, 0);
	surface = SDL_GetWindowSurface(window);

	while(1) {
		 x = rand()%32767;
		 y = rand()%32767;
		x0 = 1.0*x/(32767/4.0) - 2.0;
		y0 = 1.0*y/(32767/4.0) - 2.0;
		zr = 0.0;
		zi = 0.0;
		for(n=0; n<20; n++) { /**** z = z^2 + z0 ****/
			xt = zr;
			yt = zi;
			zr = xt*xt-yt*yt + x0;
			zi = 2*xt*yt + y0;
		}
		if(sqrt(zr*zr+zi*zi)>4) {
			zr = 0.0;
			zi = 0.0;
			for(n=0; n<25; n++) {
				xt = zr;
				yt = zi;
				zr = xt*xt-yt*yt + x0;
				zi = 2*xt*yt + y0;
				x = X/1.7 + 3.0*X/16*zr;
				y = Y/2.0 + 3.0*Y/9*zi;
				if( (x>-1) && (x<X) && (y>-1) && (y<Y) && plane[x + y*X]<255 )
					plane[x + y*X]++;
			}
		}
		if(!(++counter%500000)) {
			/*
			sprintf(filename, "%05d.raw", c++);
			f = fopen(filename, "wb");
			*/
			for(i=0; i<X*Y; i++) {
				p = (int *)surface->pixels+i;
              	*p = 65536*plane[i] + 256*plane[i] + plane[i];
				// fprintf(f, "%c", (unsigned char)(plane[i]));
			}
			/*
			fflush(f);
			fclose(f);
			*/
			SDL_UpdateWindowSurface(window);
		}
        	if(SDL_PollEvent(&event) && event.type == SDL_QUIT) break;
	}
	free(plane);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}
