/*
 * A391597  a(n) is the number of quadratic polynomials with coefficients in
 *          {-n, ..., n}, positive leading coefficient, and having two integer
 *          roots.
 * https://oeis.org/A391597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{u=1..n} b(floor(n/u)) where b(m) is the number of
 *            unordered integer pairs (r,s) with r ≤ s, |r+s| ≤ m and |r*s| ≤
 *            m
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Count pairs (r,s) for a given m (k).  The returned value fits in seq_t. */
static seq_t count_pairs_for_m(idx_t m)
{
    seq_t total = 0;

    /* r = 0: s ranges from 0 to m (since r ≤ s and |s| ≤ m) */
    total += (seq_t)(m + 1);

    /* Positive r: 1 ≤ r ≤ m */
    for (idx_t r = 1; r <= m; ++r) {
        /* s must satisfy r ≤ s ≤ min( floor(m/r), m - r ) */
        idx_t smax1 = m / r;          /* from |r*s| ≤ m   */
        idx_t smax2 = m - r;          /* from r+s ≤ m      */
        idx_t smax = (smax1 < smax2) ? smax1 : smax2;
        if (smax >= r)                /* ensure the interval is non‑empty */
            total += (seq_t)(smax - r + 1);
    }

    /* Negative r: -m ≤ r ≤ -1  (use signed type for r) */
    for (int64_t nr = -(int64_t)m; nr <= -1; ++nr) {
        int64_t p       = -(int64_t)nr;           /* |r| */
        int64_t m_div   = (int64_t)(m / (idx_t)p);/* floor(m/|r|) = smax */
        int64_t smax    = m_div;

        /* Lower bounds from the constraints */
        int64_t l1 = nr;                     /* r <= s                         */
        int64_t l2 = -m_div;                 /* |s| ≤ m_div => s >= -m_div     */
        int64_t l3 = -(int64_t)m - nr;       /* r + s ≥ -m  => s >= -m - r     */
        int64_t s_low = l1;
        if (l2 > s_low) s_low = l2;
        if (l3 > s_low) s_low = l3;

        /* Upper bound from |s| ≤ m_div */
        int64_t s_high = smax;

        if (s_low <= s_high) {
            int64_t diff = s_high - s_low;    /* ≥ 0 */
            total += (seq_t)(diff + 1);
        }
    }

    return total;
}

/* Cache for the b(m) values */
static seq_t *b_cache = NULL;   /* b_cache[k] = b(k) for 0 ≤ k ≤ b_cap */
static idx_t b_cap   = 0;

/* Ensure b_cache[0..up_to] is properly filled */
static void ensure_b_cache(idx_t up_to)
{
    if (up_to <= b_cap) return;

    seq_t *new_cache;
    if (b_cap == 0) {
        new_cache = mem_new(seq_t, (size_t)(up_to + 1));
        if (!new_cache) return;              /* allocation failure – unlikely */
        new_cache[0] = (seq_t)0;
    } else {
        new_cache = mem_resize(seq_t, b_cache, (size_t)(up_to + 1));
        if (!new_cache) return;
    }

    for (idx_t k = b_cap + 1; k <= up_to; ++k)
        new_cache[k] = count_pairs_for_m(k);

    b_cache = new_cache;
    b_cap   = up_to;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    ensure_b_cache(n);

    seq_t sum = 0;
    for (idx_t u = 1; u <= n; ++u) {
        idx_t m = n / u;           /* floor(n/u) */
        sum += b_cache[m];
    }
    return sum;
}
