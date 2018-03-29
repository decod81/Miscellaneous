/* gcc OpenGL-GLSL-Raytrace.c -lGL -lSDL */
#include <GL/gl.h>
#include <SDL/SDL.h>

char *d =
  "float t = gl_Color.x*1000.;"
  "vec3 scene(vec2 p) {"
  "  float z = 5.;"
  "  mat3 X = mat3(1., 0., 0., 0., cos(2.*t), sin(2.*t), 0., -sin(2.*t), cos(2.*t));"
  "  mat3 Y = mat3(cos(3.*t), 0., -sin(3.*t), 0., 1., 0., sin(3.*t), 0., cos(3.*t));"
  "  vec3 c = X*Y*vec3(1.5*cos(.5*5.*t), 1.5*sin(.5*7.*t), 7.);"
  "  while(z<10.) {"
  "    vec3 r = X*Y*(vec3(sqrt(z)*p.x, sqrt(z)*p.y, z));"
  "    if(abs(r.x-c.x)<.8 && abs(r.y-c.y)<.8 && abs(r.z-c.z)<.8) return r;"
  "    if(distance(r, c)<1.) return r;"
  "    z += .05;"
  "  }"
  "  return vec3(10.);"
  "}"
  "void main() {"
  "  gl_FragColor = vec4(0.);"
  "  vec2 p = vec2(gl_FragCoord.x/256.-1., gl_FragCoord.y/256.-1.);"
  "  vec3 q = scene(p);"
  "  if(length(q)<10.) {"
  "    vec3 dx = scene(vec2(p.x + .05, p.y)) - scene(vec2(p.x - .05, p.y));"
  "    vec3 dy = scene(vec2(p.x, p.y + .05)) - scene(vec2(p.x, p.y - .05));"
  "    float a = 1.5*dot(normalize(cross(dx, dy)), normalize(q))/sqrt(length(q));"
  "    gl_FragColor = vec4(a);"
  "  }"
  "}";

main(c) {
  SDL_Event e;
  SDL_SetVideoMode(512, 512, 0, SDL_OPENGL);
  glUseProgram(glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &d));

/*
  c = 0;
  GLubyte **pixels = malloc(3*512*512);
  int cur;
  char filename[80];
*/

  while(e.type!=SDL_QUIT) {
    glColor3us(c++, 0, 0);
    glRecti(-1, -1, 1, 1);
    SDL_GL_SwapBuffers();
    SDL_PollEvent(&e);
/*
    sprintf(filename, "%04d.ppm", c);
    FILE *f = fopen(filename, "w");
    fprintf(f, "P3\n%d %d\n%d\n", 512, 512, 255);
    *pixels = realloc(*pixels, 3*512*512);
    glReadPixels(0, 0, 512, 512, GL_RGB, GL_UNSIGNED_BYTE, *pixels);
    for(int i=0; i<512; i++) {
        for(int j=0; j<512; j++) {
            cur = 3*((512-i-1)*512+j);
            fprintf(f, "%3d %3d %3d ", (*pixels)[cur], (*pixels)[cur+1], (*pixels)[cur+2]);
        }
        fprintf(f, "\n");

    }
*/
  }
}
