/*
 * A343561  2nd row of A341867: a(n) = (n^2+15*n+32)*2^(n-3).
 * https://oeis.org/A343561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+15*n+32)*2^(n-3) Offset: 0 (a(0) is the first term)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute the polynomial n^2 + 15*n + 32 in seq_t arithmetic */
    seq_t nseq = (seq_t)n;
    seq_t num = nseq * nseq + 15 * nseq + 32;

    /* For n = 0,1,2 the exponent is negative, so divide by 8,4,2 */
    if (n < 3u) {
        unsigned int div = 1u << (3u - (unsigned)n); /* 8,4,2 */
        return num / (seq_t)div;
    }

    /* For n >= 3 multiply by 2^(n-3) */
    seq_t pow = pow2((idx_t)(n - 3));
    return num * pow;
}
