/*
 * A276418  Starting a random walk on Z at 0 triangle T(j,k) gives the number
 *          of paths of length 2*j returning to 0 exactly k times.
 * https://oeis.org/A276418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, walk, easy
 * Formula:   a(n) = T(j,k) where j = largest integer with j(j+1)/2 <= n, k =
 *            n - j(j+1)/2, and T(j,k) = 2^k * C(2j-k, j-k)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 135, u64 n <= 594,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

static inline idx_t triangular(idx_t j) {
    return j * (j + 1) / 2;
}

seq_t a(idx_t n) {
    /* Find the row j of the triangle */
    idx_t j = 0;
    while (triangular(j + 1) <= n) {
        ++j;
    }

    /* Column index k */
    idx_t k = n - triangular(j);

    /* Compute T(j,k) = 2^k * C(2j-k, j-k) */
    seq_t term_pow  = pow2(k);                     /* 2^k */
    seq_t term_binom = binomial(2 * j - k, j - k); /* C(2j-k, j-k) */
    return term_pow * term_binom;
}
