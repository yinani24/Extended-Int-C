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
    int store_length = strlen(s);
    int prev = store_length;
    for(int i = 0; i < (int) ap->size; i++){
        char temp[15];
        int shifter = store_length - (i+1)*8;
        if( shifter < 0){
            shifter = 0;
        }
        memmove(temp, s + shifter, prev-shifter);
        temp[prev-shifter] = '\0';
        uint32_t int_temp = strtoul(temp, NULL, 16);
        ap->bytes[k] = int_temp;
        prev = shifter;
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
    
    APInt store_src1, store_src2;
    store_src1.size = src1->size;
    store_src2.size = src2->size;
    
    store_src1.bytes = malloc(store_src1.size * sizeof(uint32_t));
    store_src2.bytes = malloc(store_src2.size * sizeof(uint32_t));

    for(uint64_t i = 0; i < store_src1.size; i++){
        store_src1.bytes[i] = src1->bytes[i];
    }

    for(uint64_t i = 0; i < store_src2.size; i++){
        store_src2.bytes[i] = src2->bytes[i];
    }

    if(dest->size){
        free(dest->bytes);
    }

    uint64_t lim = 0;
    uint64_t big = 0;

    if(store_src1.size < store_src2.size){
        lim = store_src1.size;
        big = store_src2.size;
    }
    else{
        lim = store_src2.size;
        big = store_src1.size;
    }

    uint32_t temp_arr[big+1];
    
    uint32_t carry = 0;

    for(uint64_t i = 0; i<lim; i++){
        uint32_t temp_carry = carryover(store_src1.bytes[i],store_src2.bytes[i], carry);
        uint32_t up;
        if(temp_carry){
            uint64_t bekh = (store_src1.bytes[i]) + (store_src2.bytes[i]) + carry;
            up = (bekh) & 0xFFFFFFFF;
        }
        else{
            up = (store_src1.bytes[i]) + (store_src2.bytes[i]) + carry;
        }
        carry = temp_carry;
        temp_arr[i] = up;
    }

    for(uint64_t i = lim; i < big; i++){
        if(store_src1.size < store_src2.size){
            uint32_t temp_carry = carryover(store_src2.bytes[i], 0, carry);
            uint32_t up;
            if(temp_carry){
                uint64_t bekh = (store_src2.bytes[i]) + carry;
                up = (bekh) & 0xFFFFFFFF;
            }
            else{
                up = (store_src2.bytes[i]) + carry;
            }
            carry = temp_carry;
            temp_arr[i] = up;
        }
        else{
            uint32_t temp_carry = carryover(store_src1.bytes[i], 0, carry);
            uint32_t up;
            if(temp_carry){
                uint64_t bekh = (store_src1.bytes[i]) + carry;
                up = (bekh) & 0xFFFFFFFF;
            }
            else{
                up = (store_src1.bytes[i]) + carry;
            }
            carry = temp_carry;
            temp_arr[i] = up;
        }
    }

    if(carry){
        dest->size = big + 1;
        temp_arr[dest->size - 1] = carry;
    }
    else{
        dest->size = big;
    }
     
    dest->bytes = malloc(dest->size * sizeof(uint32_t));

    for(uint64_t j = 0; j < dest->size; j++){
        dest->bytes[j] = temp_arr[j];    
    }


    if(store_src1.size){
        free(store_src1.bytes);
    }

    if(store_src2.size){
        free(store_src2.bytes);
    }
}

void cmp(APInt * op1, APInt * op2){
    if(op1->size < op2->size){
        printf("%d\n", -1);
    }
    else if(op1->size > op2->size){
        printf("%d\n", 1);
    }
    else if(op1->size == op2->size){
        int count = 0;
        for(int i = op1->size - 1; i >= 0; i--){
            if(op1->bytes[i] < op2->bytes[i]){
                printf("%d\n", -1);
                count = 1;
                break;
            }
            else if(op1->bytes[i] > op2->bytes[i]){
                printf("%d\n", 1);
                count = 1;
                break;
            }
        }

        if(!count){
            printf("%d\n", 0);
        }

    }
}

void SHL(APInt * dst, APInt * src, uint64_t k){
    
    APInt store_src;
    store_src.size = src->size;
    store_src.bytes = malloc(store_src.size * sizeof(uint32_t));

    for(uint64_t i = 0; i < store_src.size; i++){
        store_src.bytes[i] = src->bytes[i];
    }

    if(dst->size){
        free(dst->bytes);
    }

    uint64_t shift = k/32;
    uint64_t track = 0;

    dst->bytes = malloc(sizeof(uint32_t) * shift);

    for(uint64_t i = 0; i < shift; i++){
        dst->bytes[i] = 0x00000000;
        k -= 32;
    }

    uint32_t rev = 0;
    uint32_t temp_store_arr[src->size + 1];
    uint32_t store, temp;

    for(uint64_t i = 0; i < src->size; i++){
        store = store_src.bytes[i];
        temp = store_src.bytes[i];
        uint32_t result = store >> (32 - k);
        temp <<= k;
        temp |= rev;
        rev = result;
        temp_store_arr[i] = temp;
        track++;
    }

    if(rev != 0){
        temp_store_arr[track] = rev;
        track++; 
    }

    dst->bytes = realloc(dst->bytes, sizeof(uint32_t) * (shift + track));
    
    for(uint64_t k = shift, q = 0; k < (track + shift); k++, q++){
        dst->bytes[k] = temp_store_arr[q];
    }

    dst->size = track + shift;
    
    if(store_src.size){
        free(store_src.bytes);
    }
}

// void mul_uint64(APInt * ){

// }

void destroy_APInt(APInt * ap){
    //printf("APSize: %d\n", ap->size);
    if(ap->size != 0){
        free(ap->bytes);
        ap->size = 0;
    }
}

//make it write to the file
void dump(APInt * arr, int num_size){
    // change the dump function completely
    for(int i = 0; i < num_size; i++){
        if(arr[i].size){
            
            //out_count = 0;
            
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
                            printf("%02x", hello);
                        }
                    }

                }
                else{
                    
                    for(int k = 0; k < 4; k++){
                        printf("Hello");
                        uint8_t hello = (x >> (32-8*(k+1))) & 0xff;
                        printf("%02x", hello);
                    }

                }
            }
            
            printf("\n");
        }
    }
    //printf("\n");
}

// ask about carryover is 1 or not in addition
// multiplication try to clarify some doubts
// try to ask help for dump
// try to ask help for SHL


