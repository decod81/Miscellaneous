#include <stdio.h>

/**** arbitrary precision arithmetic ****/
#define PRECISION 40 /* bytes */
#define NUMS 256     /* number of digits we attempt to print */

struct arb {
  unsigned char array[PRECISION];
};

arb_zero(struct arb *a) {
  int i;
  
  for(i=0; i<PRECISION; i++)
    a->array[i] = 0;
}

lshift(struct arb *a) {
  int i;
  
  for(i=PRECISION-1; i>0; --i)
    a->array[i] = (a->array[i] << 1) | (a->array[i-1] >> 7);
  a->array[0] = a->array[0] << 1;
}

rshift(struct arb *a) {
  int i;
  
  for(i=0; i<PRECISION-1; ++i)
    a->array[i] = (a->array[i] >> 1) | (a->array[i+1] << 7);
  a->array[PRECISION-1] = a->array[PRECISION-1] >> 1;
}

lshift_byte(struct arb *a, int n) {
  int i;
  
  for(i=PRECISION-1; i>=n; i--)
    a->array[i] = a->array[i-n];
  for(; i>=0; i--)
    a->array[i] = 0;
}

rshift_byte(struct arb *a, int n) {
  int i;
  
  for(i=0; i<n; i++)
    a->array[i] = a->array[i+1];
  for(; i<PRECISION; i++)
    a->array[i] = 0;
}

arb_add(struct arb *a, struct arb *b, struct arb *c) {
  int i, j, carry = 0;
  
  for(i=0; i<PRECISION; i++) {
    j = a->array[i] + b->array[i] + carry;
    carry = (j > 255);
    c->array[i] = (j & 255);
  }
}

arb_sub(struct arb *a, struct arb *b, struct arb *c) {
  int borrow = 0;
  int i, j, k, l;
  
  for(i=0; i<PRECISION; i++) {
    j = (int)a->array[i] + 256;
    k = (int)b->array[i] + borrow;
    l = (j - k);
    c->array[i] = (int)(l & 255);
    borrow = (l<256);
  }
}

int2arb(int i, struct arb *n) { /* sign not included so this only works for the first 31 bits */
  arb_zero(n);
  n->array[0] = (i & 0x000000ff);
  n->array[1] = (i & 0x0000ff00) >> 8;
  n->array[2] = (i & 0x00ff0000) >> 16;
  n->array[3] = (i & 0xff000000) >> 24;
}

arb_mul(struct arb *a, struct arb *b, struct arb *c) {
  int i, j;
  struct arb A;
  struct arb B;
  
  arb_zero(c);
  for(i=0; i<PRECISION; i++) {
    arb_zero(&A);
    for(j=0; j<PRECISION; j++) {
      if(i+j<PRECISION) {
        arb_zero(&B);
        int2arb((int)a->array[i] * (int)b->array[j], &B);
        lshift_byte(&B, i + j);
        arb_add(&B, &A, &A);
      }
    }
    arb_add(c, &A, c);
  }
}

arb2arb(struct arb *a, struct arb *b) {
  int i;
  
  for(i=0; i<PRECISION; i++)
    b->array[i] = a->array[i];
}

int arb_is_zero(struct arb *n) {
  int i;
  
  for(i=0; i<PRECISION; i++)
    if(n->array[i])
      return 0;
  return 1;
}

arb_or(struct arb *a, struct arb *b, struct arb *c) {
  int i;
  
  for(i=0; i<PRECISION; i++)
    c->array[i] = (a->array[i] | b->array[i]);
}

int arb_cmp(struct arb *a, struct arb *b) {
  int i = PRECISION;
  
  do {
    i = i - 1;
    if(a->array[i]>b->array[i])
      return 1;
    else if(a->array[i]<b->array[i])
      return -1;
  } while(i!=0);
  return 0;
}

arb_div(struct arb *a, struct arb *b, struct arb *c) {
  struct arb A;
  struct arb B;
  struct arb denom;
  int overflow = 0;

  int2arb(1, &A);
  arb2arb(b, &denom);
  arb2arb(a, &B);
  while(arb_cmp(&denom, a) != 1) {
    if(denom.array[PRECISION-1] >= 128) {
      overflow = 1;
      break;
    }
    lshift(&A);
    lshift(&denom);
  }
  if(!overflow) {
    rshift(&denom);
    rshift(&A);
  }
  arb_zero(c);
  while(!arb_is_zero(&A)) {
    if(arb_cmp(&B, &denom)!=-1) {
      arb_sub(&B, &denom, &B);
      arb_or(c, &A, c);
    }
    rshift(&A);
    rshift(&denom);
  }
}

arb_mod(struct arb *a, struct arb *b, struct arb *c) {
  struct arb A;

  arb_div(a, b, c);
  arb_mul(c, b, &A);
  arb_sub(a, &A, c);
}

arb_and(struct arb *a, struct arb *b, struct arb *c) {
  int i;
  for(i=0; i<PRECISION; i++)
    c->array[i] = (a->array[i] & b->array[i]);
}

arb_xor(struct arb *a, struct arb *b, struct arb *c) {
  int i;
  for(i=0; i<PRECISION; i++)
    c->array[i] = (a->array[i] ^ b->array[i]);
}

printarb(struct arb *a) {
  int i;
  struct arb b;
  struct arb c;
  struct arb d;
  char n[NUMS];
   
  int2arb(10, &b);
  int2arb(10, &c);
  for(i=0; i<NUMS; i++) {
    arb_mod(a, &c, &d);
    n[i] = d.array[0]+'0';
    arb_sub(a, &d, a);
    arb_div(a, &b, a);
  }
  for(i=0; i<NUMS; i++)
    if(n[NUMS-1-i]!='0') break;
  for(; i<NUMS; i++)
    printf("%c", n[NUMS-1-i]);
  printf("\n");
}
/**** arbitrary precision arithmetic ****/

int main() {
  struct arb a;
  struct arb b;
  struct arb c;
  char s[256];
  int i;
 
  arb_zero(&a);
  arb_zero(&b);
  arb_zero(&c);
  
  srand(time(NULL));
  for(i=0; i<PRECISION; i++)
    a.array[i] = rand()%256;
   
  //arb_add(&a, &b, &c);
  //arb_mul(&a, &b, &c);
  //arb_div(&a, &b, &c);
  
  printarb(&a);
}
