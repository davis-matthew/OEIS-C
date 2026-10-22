/*
 * A342307  Table read by ascending antidiagonals: T(n, k) is the maximum
 *          number of quasi k-gons that are not k-gons in a finite projective
 *          plane of order n, with k >= 3.
 * https://oeis.org/A342307
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 25, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   factorial_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A342307: T(n, k) = k!*C(k-1,2)*C(n^2+n+1, k-1)*(n-1) read by ascending antidiagonals.
 * a(n) returns the n‑th term for n ≥ 2 (a(2)=126). Implementation does not use any
 * external caching and works for all supported seq_t types.
 */

#include <stdint.h>
#include "../utils/factorial_small.h"

/* ---------- small helper functions ---------- */

/* multiplicative binomial coefficient: C(n, k) */
static seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r *= (n - k + i);
        r /= i;          /* division is exact */
    }
    return r;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* undefined for indices < 2 */

    /* zero‑based position in the antidiagonal reading */
    idx_t pos = n - 2;

    /* find minimal t with t*(t+1)/2 > pos */
    idx_t t = 1;
    while ((t * (t + 1)) / 2 <= pos)
        ++t;

    /* compute coordinates (n_val, k_val) for this term */
    idx_t s = t + 4;                           /* sum n+k */
    idx_t offset = pos - ((t - 1) * t) / 2;    /* position within this antidiagonal */
    idx_t k_val = 3 + offset;                  /* k = 3 + offset */
    idx_t n_val = s - k_val;                   /* n = s - k */

    /* auxiliary value m = n^2 + n + 1 */
    idx_t m = n_val * n_val + n_val + 1;

    /* compute T(n, k) = k!*C(k-1,2)*C(m, k-1)*(n-1) */
    seq_t val = factorial_small(k_val);
    val *= binomial_small(k_val - 1, 2);
    val *= binomial_small(m, k_val - 1);
    val *= (seq_t)(n_val - 1);

    return val;
}
