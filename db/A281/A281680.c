/*
 * A281680  a(0)=1; for n > 0, if 2n+1 is prime, then a(n)=1, otherwise a(n) =
 *          (2n+1)/(largest proper divisor of 2n+1).
 * https://oeis.org/A281680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1) / (largest proper divisor of (2*n+1)), with a(0)
 *            = 1. For composite (2*n+1) the value equals its smallest prime
 *            factor
 * Range:     u8 n <= 625, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lpf.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Compute 2*n+1 in seq_t to avoid overflow. */
    seq_t m = (seq_t)n * 2 + 1;

    /* Find the smallest prime factor of m. */
    idx_t m64 = (idx_t)m;          /* m fits in 64 bits for n≤10⁴. */
    uint64_t spf = lpf(m64);

    /* If spf equals m, then m is prime => return 1. */
    if (spf == m64) return 1;

    /* For composite m, the required value is the smallest prime factor. */
    return (seq_t)spf;
}
