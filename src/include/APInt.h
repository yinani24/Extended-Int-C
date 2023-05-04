#ifndef APINT_H
#define APINT_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct APInt {
  uint64_t size;
  uint32_t * bytes;
} APInt;

/* You code to declare any methods you deem necessary here. */
void destroy_APInt(APInt * ap);
void conversion_from_uint64(APInt * ap, uint64_t x);
void clone(APInt * ap1, APInt * ap2);
void conversion_hexstring_APInt(APInt * ap, char * s);
void ADD(APInt * dest, APInt * src1, APInt * src2);
void dump(APInt * arr, int num_size);
void SHL(APInt * dst, APInt * src, uint64_t k);
void cmp(APInt * op1, APInt * op2);
void mul_uint64(APInt * dst, APInt * src, uint64_t k);
void mul_APInt(APInt * dest, APInt * src1, APInt * src2);
void power(APInt * dst, APInt * src, uint64_t k);
#endif