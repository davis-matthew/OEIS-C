/*
 * A319209  a(n) = 1*2*3*4*5*6*7*8 + 9*10*11*12*13*14*15*16 +
 *          17*18*19*20*21*22*23*24 + ... + (up to n).
 * https://oeis.org/A319209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 23, u64 n <= 223,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A319209: 
   a(n) = Sum_{i=1..⌊n/8⌋} Π_{k=0..7} (8(i-1)+1 + k) 
         + (if n mod 8 > 0) Π_{t=8⌊n/8⌋+1}^{n} t
   (i.e. the sum of the products of consecutive blocks of eight,
    plus the product of the last partial block, if any).
*/

#include <stdint.h>   /* for uintptr_t and standard integer types */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1, but guard against 0 */
        return 0;

    idx_t m    = n / 8;    /* number of complete 8‑integer blocks */
    idx_t r    = n % 8;    /* size of the last (possibly empty) block */
    seq_t sum  = 0;        /* running sum of block products */

    /* sum the products of each full block */
    for (idx_t i = 0; i < m; ++i) {
        idx_t start = 8 * i + 1;          /* first integer of this block */
        seq_t prod  = 1;
        for (idx_t j = 0; j < 8; ++j) {
            prod *= (seq_t)(start + j);
        }
        sum += prod;
    }

    /* add the product of the remaining numbers, if any */
    if (r > 0) {
        idx_t start = 8 * m + 1;          /* first integer of the last block */
        seq_t part  = 1;
        for (idx_t t = start; t <= start + r - 1; ++t) {
            part *= (seq_t)t;
        }
        sum += part;
    }

    return sum;
}
