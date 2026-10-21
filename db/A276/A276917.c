/*
 * A276917  Numbers obtained by alternatively adding centered pentagonal
 *          layers of 5*(2^n-1) and 5*(3^n-1) elements.
 * https://oeis.org/A276917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*((2^{ceil(n/2)+1}-1)+((3^{floor(n/2)+1}-1)/2)) - 5*n -
 *            9
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 37, u64 n <= 77, u128 n <= 157,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

/* Compute A276917 terms, with a(0)=1, a(1)=6, ... */
seq_t a(idx_t n)
{
    /* k2 = ceil(n/2), k3 = floor(n/2) */
    idx_t k2 = (n + (n & 1)) >> 1;     /* (n + n%2) / 2  */
    idx_t k3 = n >> 1;                 /* n / 2          */

    /* Geometric sums:  Σ_{i=0..k2} 2^i = 2^{k2+1}-1
                      Σ_{j=0..k3} 3^j = (3^{k3+1}-1)/2           */
    seq_t sum2 = pow2(k2 + 1) - (seq_t)1;
    seq_t sum3 = (pow3(k3 + 1) - (seq_t)1) / (seq_t)2;

    seq_t tmp  = sum2 + sum3;                /* temp = sum2 + sum3 */
    seq_t res  = (seq_t)5 * tmp;             /* 5*(sum2+sum3) */
    res        = res - (seq_t)5 * (seq_t)n;  /* -5*n */
    res        = res - (seq_t)9;             /* -9 */

    return res;
}
