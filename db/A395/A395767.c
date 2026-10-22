/*
 * A395767  Minimal value of p(1) + Sum_{i=2...n} p(i-1)*p(i), as p ranges
 *          over all permutations of powers of 2 {1,2,4,...,2^(n-1)}.
 * https://oeis.org/A395767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(1)=1, a(2)=3, a(3)=8; for n>=3 a(n)=2^(n-3) * ( 2*n + 1 +
 *            floor(n/2) )
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n=0, return 0 safely */

    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)3;

    /* n >= 3 */
    seq_t p = pow2(n - 3);                   /* 2^(n-3) */
    seq_t two_n = (seq_t)n * (seq_t)2;       /* 2*n */
    seq_t half = (seq_t)(n / 2);             /* floor(n/2) */
    seq_t mul = two_n + (seq_t)1 + half;     /* 2*n + 1 + floor(n/2) */

    return p * mul;
}
