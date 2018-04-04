/*
 * ..\cl.exe /FA /c intro1k.c /I ..\include /Oi /fp:fast /QIfist
 * ..\crinkler /TINYHEADER /TINYIMPORT intro1k.obj ..\opengl32.lib ..\msvcrtold.lib ..\kernel32.lib ..\user32.lib ..\gdi32.lib ..\winmm.lib /SUBSYSTEM:WINDOWS /HASHTRIES:1000 /HASHSIZE:1000 /ORDERTRIES:1000 /NOINITIALIZERS /UNSAFEIMPORT /COMPMODE:SLOW /TRUNCATEFLOATS:13 /SATURATE /OUT:intro1k.exe
 */

#include <windows.h>
#include <gl\gl.h>
#include <gl\glext27118.h>

HDC h;
PIXELFORMATDESCRIPTOR p = {0, 0, PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER};
unsigned char a[1<<17], b[1<<17], c[1<<17];

char *g =
"sampler2D yy;"
"float xx,x,y,xy;"
"void main(){"
"while(x*x+y*y<10&xy++<64)xx=x*x-mod(y*y,1)+(gl_FragCoord.x/250-3)/exp(gl_Color*100)-1.4,y=(2+.2*sin(gl_Color*1000))*x*y+(gl_FragCoord.y/250-2.15)/exp(gl_Color*100),x=xx;x*x+y*y>10?gl_FragColor=1-.35*log(xy):(gl_FragColor=texture(yy,gl_FragCoord/2560-1.));}";

WinMainCRTStartup() {
	int x, y, t, rp, prg0;

	h = GetDC(CreateWindowA((LPCSTR)0xC018, 0, WS_POPUP|WS_VISIBLE|WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0));
	SetPixelFormat(h, ChoosePixelFormat(h, &p), &p);
	wglMakeCurrent(h, wglCreateContext(h));
	ShowCursor(0);

	prg0 = (wglGetProcAddress("glCreateShaderProgramv"))(GL_FRAGMENT_SHADER, 1, &g);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for(y=0; y<3700; y++) {
		for(x=512; x<65536; x++) {
			if(x<1024) a[x+25600] = 255-rand()%100;
			if(a[x]>(t=rand()%11)) a[x-256+(t&1)] = a[x]-t;
			b[x<<1] = a[x];
			b[x-256] = (b[x-257]+b[x-255]+b[x-512]+b[x])>>2;
		}
		for(x=512; x<65536; x++) c[x] = b[x+25600];
		glTexImage2D(GL_TEXTURE_2D, 0, 4, 256, 256, 0, GL_RED, GL_UNSIGNED_BYTE, c);
		(wglGetProcAddress("glUseProgram"))(prg0);
		glLoadIdentity();
		glColor3us(y, 0, 0);
		glRecti(-1, -1, 1, 1);

		(wglGetProcAddress("glUseProgram"))(0);
		glRotatef(y*1.5, 0, 0, 1);
		glBegin(GL_TRIANGLES);
			glColor4f( 0, 0, y*.02, 0.5);
			glVertex3f( 0.2,  2.0,  0.0);
			glVertex3f(-0.2,  2.0,  0.0);
			glVertex3f( 0.0,  0.0,  0.0);
		glEnd();

		SwapBuffers(h);
		PeekMessageA(0, 0, 0, 0, PM_REMOVE);

		if(GetAsyncKeyState(VK_ESCAPE)) ExitProcess(0);
	}
	ExitProcess(0);
}