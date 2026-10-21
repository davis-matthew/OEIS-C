/*
 * A290699  a(n) = 2^n - n + n^2.
 * https://oeis.org/A290699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - n + n^2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(0) = 0 (offset 0) */

/* Include the helper that computes powers of two */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    seq_t N   = (seq_t)n;          /* cast index to seq_t for arithmetic */
    seq_t p2  = pow2(n);           /* compute 2^n */
    return p2 - N + N * N;         /* a(n) = 2^n - n + n^2 */
}
