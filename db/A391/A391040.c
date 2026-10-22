/*
 * A391040  Area of the Pythagoras tree with triangles formed by successive
 *          stages of the fractal filled in.
 * https://oeis.org/A391040
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391040: Area of the Pythagoras tree (digits of the limit)
   a(n) = decimal digit at position n-1 of the real number
   80605706690966227797244468721167282379053 / 5025557197283613113718784555470007578789
   Offset: 2  (a(2) is the first digit of the representation) */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* Small fixed‑size big integer (up to 192 bits) */
typedef struct { uint64_t w[3]; } Big;

static void big_zero(Big *b)          { b->w[0]=b->w[1]=b->w[2]=0; }
static int  big_cmp(const Big *a,const Big *b) {
    for(int i=2;i>=0;--i)
        if(a->w[i]!=b->w[i]) return (a->w[i]>b->w[i])?1:-1;
    return 0;
}
static void big_mul_small(const Big *a,uint64_t m,Big *out) {
    __uint128_t carry=0;
    for(int i=0;i<3;++i){
        __uint128_t prod=(__uint128_t)a->w[i]*m+carry;
        out->w[i]=(uint64_t)prod;
        carry=(uint64_t)(prod>>64);
    }
}
static void big_mul_small_inplace(Big *a,uint64_t m) {
    __uint128_t carry=0;
    for(int i=0;i<3;++i){
        __uint128_t prod=(__uint128_t)a->w[i]*m+carry;
        a->w[i]=(uint64_t)prod;
        carry=(uint64_t)(prod>>64);
    }
}
static void big_add_small_inplace(Big *a,uint64_t v) {
    __uint128_t sum=(__uint128_t)a->w[0]+v;
    a->w[0]=(uint64_t)sum;
    uint64_t carry=(uint64_t)(sum>>64);
    for(int i=1;i<3;++i){
        __uint128_t s=(__uint128_t)a->w[i]+carry;
        a->w[i]=(uint64_t)s;
        carry=(uint64_t)(s>>64);
    }
}
static void big_sub(const Big *a,const Big *b,Big *out) {
    __int128_t borrow=0;
    for(int i=0;i<3;++i){
        __int128_t diff=(__int128_t)a->w[i]-b->w[i]-borrow;
        out->w[i]=(uint64_t)diff;
        borrow=(diff<0)?1:0;
    }
}
static void parse_big(const char *s,Big *out) {
    big_zero(out);
    for(size_t i=0; s[i]; ++i){
        char c=s[i];
        if(c<'0' || c>'9') continue;
        big_mul_small_inplace(out,10);
        big_add_small_inplace(out,c-'0');
    }
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    static int initialized = 0;
    static Big numerator, denominator, remainder, temp;
    static uint64_t int_part = 0;
    static uint8_t *digits = NULL;
    static size_t digits_cap = 0, digits_len = 0;

    if (!initialized) {
        /* Parse rational constants */
        parse_big("80605706690966227797244468721167282379053", &numerator);
        parse_big("5025557197283613113718784555470007578789", &denominator);

        /* Compute integer part by repeated subtraction (quotient is tiny) */
        Big tmp = numerator;
        int_part = 0;
        while (big_cmp(&tmp, &denominator) >= 0) {
            Big subRes;
            big_sub(&tmp, &denominator, &subRes);
            tmp = subRes;
            int_part++;
        }
        remainder = tmp;   /* p mod q */

        /* Prepare digit storage */
        digits_cap = 2048;
        digits = (uint8_t*)malloc(digits_cap);
        if (!digits) return 0; /* allocation failure */

        /* Store integer part digits (most significant first) */
        if (int_part != 0) {
            uint8_t buf[20];
            int p = 0;
            uint64_t x = int_part;
            while (x > 0) {
                buf[p++] = (uint8_t)(x % 10);
                x /= 10;
            }
            for (int i = p-1; i >= 0 && digits_len < digits_cap; --i)
                digits[digits_len++] = buf[i];
        }

        initialized = 1;
    }

    idx_t idx = n - 2;  /* 0‑based index into digit array */

    /* Generate digits up to requested index */
    while (digits_len <= idx) {
        /* temp = remainder * 10 */
        temp = remainder;
        big_mul_small_inplace(&temp, 10);

        uint8_t digit = 0;
        Big prod, new_rem;
        for (int d = 9; d >= 0; --d) {
            big_mul_small(&denominator, (uint64_t)d, &prod);
            if (big_cmp(&prod, &temp) <= 0) {
                digit = (uint8_t)d;
                big_sub(&temp, &prod, &new_rem);
                remainder = new_rem;
                break;
            }
        }

        if (digits_len >= digits_cap) {
            digits_cap <<= 1;
            uint8_t *tmp = (uint8_t*)realloc(digits, digits_cap);
            if (!tmp) return 0; /* allocation failure */
            digits = tmp;
        }
        digits[digits_len++] = digit;
    }

    return (seq_t)digits[idx];
}
