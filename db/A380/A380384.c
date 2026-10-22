/*
 * A380384  a(0)=0, a(n) = 2*(a(n-1) + ceiling(n/2)) - 1 for n>0.
 * https://oeis.org/A380384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*2^n/3) - n - 1
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t v = pow2(n);
    v *= (seq_t)5;
    v /= (seq_t)3;
    v -= (seq_t)n;
    v -= (seq_t)1;
    return v;
}
