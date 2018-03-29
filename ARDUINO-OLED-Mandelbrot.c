int mandelbrot(long xx, long yy) {
  long x0, y0, x1, y1, x2, y2;
  int n, F = 1000;

  x0 = F*3*xx/128 - 7*F/2;
  y0 = F*3*yy/64 - 3*F/2;
  x1 = 0;
  y1 = 0;
  n = 0;
  while(sqrt(x1*x1+y1*y1) < F*8 && ++n < 11) {
    x2 = x1*x1/F - y1*y1/F;
    y2 = 2*x1*y1/F;
    x1 = x2 + x0;
    y1 = y2 + y0;
  }
  return n;
}

void loop() {
  unsigned char x, y, z, c;
  int xx, yy;

  for(y=0; y<8; y++) {
    LED_WrCmd(0xb0+y);
    LED_WrCmd(0x00);
    LED_WrCmd(0x10);
    xx = 0;
    for(xx=0; xx<256; xx++) {
      z = 1;
      c = 0;
      for(x=0; x<8; x++) {
        yy = x+y*8;
        if(mandelbrot(xx, yy)>10)
          c = c + z;
        z = 2*z;
      }
      LED_WrDat(c);
      xx++;
    }
  }
}
