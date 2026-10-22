/*
 * A328567  a(n) is the smallest positive integer divisible by n such that it
 *          is possible to strike out a digit from its binary expansion (apart
 *          from trailing zeros) so that the resulting number is nonzero and
 *          divisible by n.
 * https://oeis.org/A328567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * ( 2^(floor_log2(n) + 2 - v2(n)) - 1 ), where v2(n) is
 *            the exponent of the largest power of two dividing n
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static idx_t floor_log2_u64(idx_t x) {
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* not used, but safe */

    /* Count trailing zeros (v2) */
    idx_t tmp = n;
    idx_t v2 = 0;
    while ((tmp & 1) == 0) {
        ++v2;
        tmp >>= 1;
    }

    /* Compute floor(log2(n)) */
    idx_t L = floor_log2_u64(n);

    /* Exponent k for the power of two */
    idx_t k = L + 2 - v2;

    /* 2^k as seq_t */
    seq_t two_k = pow2(k);

    /* a(n) = n * (2^k - 1) */
    return (seq_t)n * (two_k - 1);
}
