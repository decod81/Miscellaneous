/* gcc OpenGL-Fire.c -lSDL2 -lGL */
#include <stdio.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>

int WinMain(HINSTANCE hInstance, HINSTANCE   hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	SDL_Event event;
	SDL_Window *window;
	SDL_GLContext context;
	unsigned int texture;
	int x, y, i, j;
	unsigned char stage1[140*100], stage2[256*200], stage3[256*256*4];

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("OpenGL-Fire", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);
 	SDL_GL_SetSwapInterval(1);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
	
	while(1) {
		for(x=1; x<140; x++) 
			stage1[140*99+x] = 255-rand()%100;
		for(y=48; y<100; y++)
			for(x=0; x<140; x++) {
				i = stage1[y*140+x];
				j = x+(y-1)*140;
				if(i<16) 
					stage1[j] = 0;
				else 
					stage1[j+rand()%2] = i-rand()%12;
			}
		for(y=48; y<100; y++)
			for(x=0; x<140; x++)
				stage2[2*x+2*y*256] = stage1[x+y*140]; 
		for(y=96; y<199; y++)
			for(x=1; x<255; x++)
				stage2[x+y*256] = (stage2[(x-1)+y*256]+stage2[x+(y-1)*256]+stage2[(x+1)+y*256]+stage2[x+(y+1)*256])>>2;
		for(y=0; y<97; y++)
			for(x=0; x<256; x++)
				stage3[4*x+4*y*256] = stage2[x+(y+103)*256];
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, stage3);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2f(-1.07, 1.07);
		glTexCoord2f(1, 0);
		glVertex2f(1.07, 1.07);
		glTexCoord2f(1, 1);
		glVertex2f(1.07, -4.5);
		glTexCoord2f(0, 1);
		glVertex2f(-1.07, -4.5);
		glEnd();
		SDL_GL_SwapWindow(window);
		if(SDL_PollEvent(&event) && event.type == SDL_QUIT) break;
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
}
