#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL2/SDL.h>

char *d =
  "float t = gl_Color.x*1000.;"
  "float PHI = 1.61803398874989484820459 * 00000.1;"
  "float PI  = 3.14159265358979323846264 * 00000.1;"
  "float SQ2 = 1.41421356237309504880169 * 10000.0;"
  "float gold_noise(in vec2 coordinate, in float seed) {"
  "  return fract(tan(distance(coordinate*(seed+PHI), vec2(PHI, PI)))*SQ2);"
  "}"
  "vec3 scene(vec2 p) {"
  "  float z = 5.;"
  "  mat3 X = mat3(1., 0., 0., 0., cos(2.*t), sin(2.*t), 0., -sin(2.*t), cos(2.*t));"
  "  mat3 Y = mat3(cos(3.*t), 0., -sin(3.*t), 0., 1., 0., sin(3.*t), 0., cos(3.*t));"
  "  vec3 c = X*Y*vec3(1.5*cos(.5*5.*t), 1.5*sin(.5*7.*t), 7.);"
  "  while(z<10.) {"
  "    vec3 r = X*Y*(vec3(sqrt(z)*p.x, sqrt(z)*p.y, z));"
  "    if(abs(r.x-c.x)<.75*1.2 && abs(r.y-c.y)<.75*1.2 && abs(r.z-c.z)<.75*1.2) return r;"
  "    if(distance(r, c)<1.*1.2) return r;"
  "    z += .0005 + gold_noise(p, t)/2e5;"
  "  }"
  "  return vec3(10.);"
  "}"
  "void main() {"
  "  gl_FragColor = vec4(0.);"
  "  vec2 p = vec2(gl_FragCoord.x/256.-1., gl_FragCoord.y/256.-1.);"
  "  vec3 q = scene(p);"
  "  if(length(q)<10.) {"
  "    float de = .0005;"
  "    vec3 dx = scene(vec2(p.x + de, p.y)) - scene(vec2(p.x - de, p.y));"
  "    vec3 dy = scene(vec2(p.x, p.y + de)) - scene(vec2(p.x, p.y - de));"
  "    float a = 2.2*dot(normalize(cross(dx, dy)), normalize(q))/sqrt(length(q));"
  "    gl_FragColor = vec4(a);"
  "  }"
  "}";

int WinMain(HINSTANCE hInstance, HINSTANCE   hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int a, b, c;
	SDL_Event event;
	SDL_Window *window;
	SDL_GLContext context;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);
 	// SDL_GL_SetSwapInterval(1);
	
	b = (wglGetProcAddress("glCreateShaderProgramv"))(GL_FRAGMENT_SHADER, 1, &d);
	(wglGetProcAddress("glUseProgram"))(b);

	/*
	a = glCreateShader(GL_FRAGMENT_SHADER);
	b = glCreateProgram();
	glShaderSource(a, 1, &d, 0);
	glCompileShader(a);
	glAttachShader(b, a);
	glLinkProgram(b);
	glUseProgram(b);
	*/
	
	while(1) {
		glColor3us(c++, 0, 0);
		glRecti(-1, -1, 1, 1);
		SDL_GL_SwapWindow(window);
		if(SDL_PollEvent(&event) && event.type == SDL_QUIT) break;
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
}
