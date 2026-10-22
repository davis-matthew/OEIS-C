/*
 * A343960  Triangle read by rows: T(n,m) = Sum_{k=1..m}
 *          (k/n)*binomial(n,m-k)*binomial(n,m), n >= m >= 1.
 * https://oeis.org/A343960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,m) where r = smallest integer with r(r+1)/2 >= n and
 *            m = n - r(r-1)/2. T(r,m) = sum_{k=1..m} (k/r) *
 *            binomial(r,m-k)*binomial(r,m)
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function binomial(idx_t n, idx_t k) from ../utils/binomial.h */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r and column m in the triangular enumeration */
    idx_t r = 1;
    idx_t cum = 1;                     /* cumulative terms up to current row */
    while (cum < n) {
        r++;
        cum += r;
    }
    idx_t prev = cum - r;              /* terms before this row */
    idx_t m = n - prev;                /* position within the row (1‑based) */

    /* T(r,m) = binomial(r,m) * ( sum_{k=1..m} k * binomial(r,m-k) ) / r */
    seq_t c_r_m = binomial(r, m);      /* binomial(r,m) */
    seq_t sum_k = 0;

    for (idx_t k = 1; k <= m; ++k) {
        seq_t term = binomial(r, m - k);   /* binomial(r,m-k) */
        term *= (seq_t)k;                 /* multiply by k */
        sum_k += term;                    /* accumulate */
    }

    seq_t result = c_r_m * sum_k;    /* multiply by binomial(r,m) */
    result /= (seq_t)r;              /* divide by r (exact division) */

    return result;
}
