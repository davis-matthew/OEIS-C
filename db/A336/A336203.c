/*
 * A336203  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          T(n,k) = Sum_{j=0..n} 2^j * binomial(n,j)^k.
 * https://oeis.org/A336203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,k) where T(n,k)=∑_{j=0..n}2^j * binomial(n,j)^k * and
 *            (i,k) are the coordinates on the antidiagonal that contain a(n)
 * Range:     u8 n <= 24, u16 n <= 49, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   binomial, ipow_i32, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Locate the diagonal d and the offset within it. */
    idx_t d = 0;            /* current diagonal */
    idx_t cumulative = 0;   /* number of terms in all previous diagonals */
    while (cumulative + (d + 1) <= n) {
        cumulative += d + 1;
        d++;
    }
    idx_t offset = n - cumulative;       /* position within diagonal d */
    idx_t i = offset;                      /* row index */
    idx_t k = d - offset;                  /* column index */

    /* Compute T(i,k) = sum_{j=0..i} 2^j * binomial(i,j)^k. */
    seq_t sum = 0;
    for (idx_t j = 0; j <= i; ++j) {
        seq_t bin = binomial(i, j);
        seq_t term = pow2(j) * ipow_i32(bin, (int)k);
        sum += term;
    }
    return sum;
}
