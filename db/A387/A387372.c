/*
 * A387372  Number of chordless cycles in the complement of the n-Hanoi graph.
 * https://oeis.org/A387372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (3^(2*n+2) - 240*3^(n-1) + 183) / 8 for n>=2, a(1)=0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    idx_t e_big   = n * 2 + 2;     /* exponent 2*n+2 */
    idx_t e_small = n - 1;         /* exponent n-1  */

    seq_t pow_big   = seq_pow((seq_t)3, e_big);   /* 3^(2*n+2)   */
    seq_t pow_small = seq_pow((seq_t)3, e_small); /* 3^(n-1)    */

    seq_t term2 = seq_mul(pow_small, (seq_t)240); /* 240*3^(n-1) */

    seq_t numerator = pow_big - term2 + (seq_t)183;

    return numerator / (seq_t)8;
}
