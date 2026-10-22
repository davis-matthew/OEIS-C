/*
 * A376102  Array read by ascending antidiagonals: A(n,k) = k*2^(n+1) + 1.
 * https://oeis.org/A376102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = let t = floor((sqrt(8*n+1)-1)/2); let prev = t*(t+1)/2;
 *            let pos = n - prev; a(n) = pos * 2^(t - pos + 1) + 1
 * Range:     u8 n <= 36, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute diagonal index t */
    idx_t t = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);

    /* cumulative count up to previous diagonal */
    idx_t prev = t * (t + 1) / 2;

    /* position inside the current diagonal (0‑based, from the bottom) */
    idx_t pos = n - prev;

    /* exponent for the power of two: i+1 where i = t - pos */
    idx_t exp = t - pos + 1;

    /* compute term: pos * 2^exp + 1 */
    seq_t pow = pow2(exp);
    return (seq_t)pos * pow + (seq_t)1;
}
