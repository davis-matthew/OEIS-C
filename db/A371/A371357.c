/*
 * A371357  a(n) = 2^(n - HammingWeight(n)) * binomial(2*n, n).
 * https://oeis.org/A371357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n - HammingWeight(n)) * binomial(2*n, n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 976 (2026-11-01)
 * Helpers:   binomial, popcount_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/popcount_idx.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^(n - bitcount(n)) */
    seq_t pow = pow2(n - popcount_idx(n));

    /* binomial(2*n, n) */
    seq_t bin = binomial(2 * n, n);

    return pow * bin;
}
