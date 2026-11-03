/*
 * A375612  Triangle read by rows: T(n, k) = n! * 4^k * hypergeom([-k], [-n],
 *          -1/4).
 * https://oeis.org/A375612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r = ⌊(−1+√(8n+1))/2⌋ and k = n – r(r+1)/2 ;
 *            T(n, k) = ∑_{j=0}^{k} (−1)^j binom(k, j)·4^{k−j}·(n−j)! This
 *            implements the triangle read by rows from OEIS A375612
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* find row r and column k */
    idx_t r = 0;
    while (r * (r + 1) / 2 <= n)
        ++r;
    --r;                          /* r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t k = n - r * (r + 1) / 2;

    /* compute T(r, k) = sum_{j=0..k} (-1)^j binom(k, j) * 4^{k-j} * (r-j)! */
    seq_t sum_pos = 0;   /* even j terms */
    seq_t sum_neg = 0;   /* odd j terms */

    for (idx_t j = 0; j <= k; ++j) {
        seq_t term = binomial(k, j);
        term = term * factorial(r - j);
        term = term * ipow_u64_idx(4, k - j);
        if ((j & 1) == 0)
            sum_pos += term;
        else
            sum_neg += term;
    }

    return sum_pos - sum_neg;   /* sum_pos ≥ sum_neg for all n, k */
}
