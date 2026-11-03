/*
 * A343535  Number T(n,k) of permutations of [n] having exactly k consecutive
 *          triples j, j+1, j-1; triangle T(n,k), n>=0, 0<=k<=floor(n/3), read
 *          by rows.
 * https://oeis.org/A343535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = sum_{i=k}^{⌊n/3⌋} (-1)^(i-k) * C(i,k) * C(n-2i,i) *
 *            (n-2i)!
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 40 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the triangle T(n,k) of A343535,
   where n≥0 and 0≤k≤⌊n/3⌋, and returns the entry read
   by rows as a single sequence a(n). */

#include <stdint.h>
#include "../utils/binomial.h"

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

seq_t a(idx_t n_flat)
{
    /* Find the row r and column k from the flattened index n_flat. */
    idx_t row = 0;
    idx_t cum = 0;
    while (1) {
        idx_t entries = row / 3 + 1;          /* number of terms in this row */
        if (cum + entries > n_flat) break;   /* target lies in this row   */
        cum += entries;
        ++row;
    }
    idx_t k = n_flat - cum;   /* column inside the row   */
    idx_t r = row;            /* row number              */

    /* Compute T(r,k) by the inclusion–exclusion formula. */
    seq_t result = 0;
    idx_t max_i = r / 3;          /* upper bound for i       */
    for (idx_t i = k; i <= max_i; ++i) {
        seq_t term = binomial(i, k);                       /* C(i,k) */
        term = term * binomial(r - 2 * i, i);              /* *C(r-2i,i)   */
        term = term * factorial(r - 2 * i);             /* *(r-2i)!     */
        if (((i - k) & 1) == 0) {
            result += term;                            /* + term         */
        } else {
            result -= term;                            /* - term         */
        }
    }
    return result;
}
