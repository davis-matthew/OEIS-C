/*
 * A241269  Denominator of c(n) = (n^2+n+2)/((n+1)*(n+2)*(n+3)).
 * https://oeis.org/A241269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, look, easy
 * Formula:   a(n) = (n+1)(n+2)(n+3)/gcd(4n-4, n^2+n+2)
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 2045, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* offset 0 */

/* This implementation uses the provided gcd routine from
   utils/gcd.h.  All arithmetic is performed with idx_t for
   intermediate values to avoid overflow in seq_t.  The function
   works for any build where seq_t is an unsigned integer type
   (including arbitrary-precision).  */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Incremented values of n */
    idx_t n1 = n + 1;
    idx_t n2 = n + 2;
    idx_t n3 = n + 3;

    /* Numerator: (n+1)(n+2)(n+3) */
    idx_t num_idx = n1 * n2 * n3;
    seq_t num = (seq_t)num_idx;

    /* Denominator components for the gcd */
    idx_t d1_idx = 4 * n - 4;            /* May underflow for n=0, handled as wrap-around */
    idx_t d2_idx = n * n + n + 2;

    seq_t g = gcd((seq_t)d1_idx, (seq_t)d2_idx);

    return num / g;
}
