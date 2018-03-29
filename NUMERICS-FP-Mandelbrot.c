/* Fixed-point arithmetic test of drawing Mandelbrot fractal using (SDL2)
 *
 * gcc NUMERICS-FP-Mandelbrot.c -lSDL2
 *
 */
#include <math.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

//int WinMain(int argc, char* args[]) {
int main() {
	SDL_Event event;
	SDL_Window *window;
	SDL_Surface *surface;
	int *p;
	short x0, y0, x1, y1, x2, y2, n;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("SDL2Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 200, 0);
	surface = SDL_GetWindowSurface(window);

	while(1) {
    		for(int y=0; y<200; y++)
			for(int x=0; x<320; x++) {
				x0 = 24*(x - 200);
				y0 = 24*(y - 100);
				x1 = 0;
				y1 = 0;
				n = 0;
				while(x1*x1+y1*y1 < 20000000 && ++n < 32) {
					x2 = (x1*x1>>11) - (y1*y1>>11);
					y2 = 2*x1*y1>>11;
					x1 = x2 + x0;
					y1 = y2 + y0;
				}
				p = (int*)surface->pixels+x+y*320;
				n = n<<3;
				if(n>255) n = 255;
	        		*p = 65536*n + 256*n + n;
      			}
		SDL_UpdateWindowSurface(window);
	        if(SDL_PollEvent(&event) && event.type == SDL_QUIT) break;
	}
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
