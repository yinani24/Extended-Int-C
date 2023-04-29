#include "APInt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void conversion_from_uint64(APInt * ap, uint64_t x){
    ap->size = 1;
    ap->bytes = malloc(sizeof(uint32_t) * 1);

    for(int i = 0; i < 2; i++){
        if(i == 1){
            uint32_t up = (x >> 32) & 0xFFFFFFFF;
            if(up != 0){
                ap->bytes = realloc(ap->bytes, 2 * sizeof(uint32_t));
                ap->bytes[i] = up;    
                ap->size++;
            }
        }
        else{
            ap->bytes[i] = x & 0xFFFFFFFF;
        }
    }    
}

//ap1 is the copy to be cloned and ap2 is the copy to clone from
void clone(APInt * ap1, APInt * ap2){
    ap1->size = ap2->size;
    ap1->bytes = malloc(sizeof(uint32_t) * ap1->size);
    
    for(uint64_t i = 0; i < ap2->size; i++){
        ap1->bytes[i] = ap2->bytes[i];
    }
}

void conversion_hexstring_APInt(APInt * ap, char * s){
    if(strlen(s) % 8 == 0){
        ap->size = strlen(s)/8;
    }
    else{
        ap->size = strlen(s)/8 + 1;
    }
    
    ap->bytes = malloc(ap->size * sizeof(uint32_t));    
    
    int k = 0;
    
    for(unsigned i = 0; i < strlen(s); i+=8){
        char temp[15];
        memcpy(temp, s + i, 8);
        uint32_t int_temp = strtoul(temp, NULL, 16);
        ap->bytes[k] = int_temp;
        k++;
    }
}

uint32_t carryover(uint32_t k, uint32_t l,uint32_t carry) {
  uint32_t sum = k + l + carry;
  uint32_t carry_over = (sum < k || sum < l) ? 1 : 0;
  return carry_over;
}

//addition with 2 APInt's 
void ADD(APInt * dest, APInt * src1, APInt * src2){
    
    uint64_t lim = 0;
    uint64_t big = 0;

    if(src1->size < src2->size){
        lim = src1->size;
        big = src2->size;
    }
    else{
        lim = src2->size;
        big = src1->size;
    }

    uint32_t temp_arr[big+1];
    
    uint32_t carry = 0;

    for(uint64_t i = 0; i<lim; i++){
        uint32_t temp_carry = carryover(src1->bytes[i],src2->bytes[i], carry);
        uint32_t up;
        if(temp_carry){
            uint64_t bekh = (src1->bytes[i]) + (src2->bytes[i]) + carry;
            up = (bekh) & 0xFFFFFFFF;
        }
        else{
            up = (src1->bytes[i]) + (src2->bytes[i]) + carry;
        }
        carry = temp_carry;
        temp_arr[i] = up;
    }

    for(uint64_t i = lim; i < big; i++){
        if(src1->size < src2->size){
            uint32_t temp_carry = carryover(src2->bytes[i], 0, carry);
            uint32_t up;
            if(temp_carry){
                uint64_t bekh = (src2->bytes[i]) + carry;
                up = (bekh) & 0xFFFFFFFF;
            }
            else{
                up = (src2->bytes[i]) + carry;
            }
            carry = temp_carry;
            temp_arr[i] = up;
        }
        else{
            uint32_t temp_carry = carryover(src1->bytes[i], 0, carry);
            uint32_t up;
            if(temp_carry){
                uint64_t bekh = (src1->bytes[i]) + carry;
                up = (bekh) & 0xFFFFFFFF;
            }
            else{
                up = (src1->bytes[i]) + carry;
            }
            carry = temp_carry;
            temp_arr[i] = up;
        }
    }

    if(carry){
        dest->size = big + 1;
    }
    else{
        dest->size = big;
    }
     
    dest->bytes = realloc(dest->bytes, dest->size * sizeof(uint32_t));

    for(uint64_t j = 0; j < dest->size; j++){
        dest->bytes[j] = temp_arr[j];    
    }

}

// void cmp(APInt * op1, APInt * op2){
//     if(op1->size < op2->size){
//         printf("%d", -1);
//     }
//     else if(op1->size > op2->size){
//         printf("%d", 1);
//     }
//     else if(op1->size == op2->size){
//         int count = 0;
//         for(uint64_t i = op1->size - 1; i >= 0; i++){
//             if(op1->bytes[i] < op2->bytes[i]){
//                 printf("%d", -1);
//                 count = 1;
//                 break;
//             }
//             else if(op1->bytes[i] > op2->bytes[i]){
//                 printf("%d", 1);
//                 count = 1;
//                 break;
//             }
//         }

//         if(!count){
//             printf("%d", 0);
//         }

//     }
// }

void SHL(APInt * dst, APInt * src, uint64_t k){
    
    uint64_t shift = k/32;
    uint64_t track = 0;

    dst->bytes = malloc(sizeof(uint32_t) * shift);

    for(uint64_t i = 0; i < shift; i++){
        dst->bytes[i] = 0x00000000;
        k -= 32;
        track++;
    }

    uint32_t rev = 0;
    uint32_t temp_store_arr[src->size + 1];
    uint32_t store, temp;

    for(uint64_t i = 0; i < src->size; i++){
        store = src->bytes[i];
        temp = src->bytes[i];
        uint32_t result = store >> (32 - k);
        temp <<= k;
        temp |= rev;
        rev = result;
        temp_store_arr[i] = temp;
        track++;
    }

    if(rev != 0){
        temp_store_arr[track] = temp;
        track++; 
    }

    dst->bytes = realloc(dst->bytes, sizeof(uint32_t) * (shift + track));
    
    for(uint64_t k = shift, q = 0; k < track; k++, q++){
        dst->bytes[k] = temp_store_arr[q];
    }

    dst->size = track;
    
}

// void mul_uint64(APInt * ){

// }

void destroy_APInt(APInt * ap){
    //printf("APSize: %d\n", ap->size);
    if(ap->size != 0){
        free(ap->bytes);
    }
}

//make it write to the file
void dump(APInt * arr, int num_size){
    // change the dump function completely
    for(int i = 0; i < num_size; i++){
        if(arr[i].size){

            for(int j = arr[i].size - 1; j >= 0; j--){
                uint32_t x = arr[i].bytes[j];
                
                if(j == (int) arr[i].size - 1){
                    int count = 0;
                    for(int k = 0; k < 4; k++){
                        uint8_t hello = (x >> (32-8*(k+1))) & 0xff;
                        if(hello != 0 && count == 0){
                           count = 1;
                            printf("0x%02x", hello);
                        }
                        else if(count == 1){
                            printf("%x", hello);
                        }
                    }                        
                }
                else{
                    for(int k = 0; k < 4; k++){
                        uint8_t hello = (x >> (32-8*(i+1))) & 0xf;
                        printf("%x", hello);
                    }
                }
            }
            printf("\n");
        }
    }
    printf("\n");
}

// ask about carryover is 1 or not in addition
// multiplication try to clarify some doubts
// try to ask help for dump
// try to ask help for SHL


