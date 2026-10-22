/*
 * A380490  Replace prime digits of n by 0's.
 * https://oeis.org/A380490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = Replace prime digits of n by 0's
 * Range:     u8 n <= 399, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static inline int is_prime_digit(unsigned char d)
{
    return d==2 || d==3 || d==5 || d==7;
}

seq_t a(idx_t n)
{
    if (n==0) return 0;
    unsigned char digits[20];
    size_t len=0;
    idx_t tmp = n;
    while (tmp>0){
        digits[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }
    seq_t res = 0;
    for (size_t i=len; i>0; ){
        --i;
        unsigned char d = digits[i];
        if (is_prime_digit(d)) d = 0;
        res = res * 10 + (seq_t)d;
    }
    return res;
}
