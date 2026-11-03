/*
 * A334009  Triangle read by rows: T(n, k) = binomial(n + k - 1, 2*k - 1) *
 *          4^(k - 1) * n/k, 1 <= k <= n.
 * https://oeis.org/A334009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binom(r + k - 1, 2*k - 1) * 4^(k - 1) * r / k, where r =
 *            smallest integer with r*(r+1)/2 >= n, and k = n - (r-1)*r/2
 * Range:     u8 n <= 8, u16 n <= 25, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* for completeness, though n≥1 */

    /* find the row r such that the cumulative count of rows ≥ n  */
    idx_t r   = 0;
    idx_t tot = 0;
    while (tot < n) {
        ++r;
        tot += r;                         /* tot = r*(r+1)/2 after loop */
    }

    /* k is the position within row r (1‑based) */
    idx_t prev = tot - r;                  /* sum of first r-1 rows */
    idx_t k    = n - prev;                 /* 1 ≤ k ≤ r */

    /* compute the triangle entry T(r,k) */
    seq_t bin = binomial(r + k - 1, 2 * k - 1);
    seq_t pow = pow4(k - 1);
    seq_t res = bin * pow;
    res = res * (seq_t)r;
    res = res / (seq_t)k;

    return res;
}
