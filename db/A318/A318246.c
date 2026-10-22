/*
 * A318246  a(n) = [x^n] Product_{k>=1} (1 + 3^n*x^k).
 * https://oeis.org/A318246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + 3^n x^k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A combinatorial reformulation: a(n) = Σ_{r≥0} P_exact(n,r) · 3^{n·r}
 * where P_exact(n,r) is the number of partitions of n into r distinct parts.
 * Using the identity P_exact(n,r) = P_at_most(n - r(r-1)/2 , r) – P_at_most(n - r(r-1)/2 , r-1),
 * we compute all needed P_at_most values by dynamic programming. */

#include <stddef.h>   /* for size_t */
#include "../utils/big_pow.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    if (n == 0) return 1; /* a(0) = 1 */

    /* compute maximum number of parts (distinct) possible for sum n */
    double discr = sqrt(8.0 * (double)n + 1.0);
    idx_t max_r = (idx_t)((discr - 1.0) * 0.5);   /* floor of (sqrt(8n+1) - 1)/2 */

    /* Allocate 2‑D table P_at_most[m][r] for 0 ≤ m ≤ n, 0 ≤ r ≤ max_r */
    size_t rsize = (size_t)max_r + 1;
    size_t rows   = (size_t)n + 1;
    seq_t *P = seq_alloc_new_array(rows * rsize);   /* will use free_seq_array later */

    /* Initialization */
    for (idx_t r = 0; r <= max_r; ++r) {
        P[r] = 1;          /* P[0][r] = 1  (empty partition of 0) */
    }
    for (idx_t m = 1; m <= n; ++m) {
        P[(size_t)m * rsize] = 0;      /* P[m][0] = 0 for m > 0 */
    }

    /* DP: P_at_most[m][r] = P_at_most[m][r-1] + P_at_most[m-r][r] */
    for (idx_t r = 1; r <= max_r; ++r) {
        for (idx_t m = 0; m <= n; ++m) {
            seq_t val = P[(size_t)m * rsize + (r - 1)];
            if (m >= r) {
                val = val + P[(size_t)(m - r) * rsize + r];
            }
            P[(size_t)m * rsize + r] = val;
        }
    }

    /* Base value of M = 3^n */
    seq_t M = big_pow(3, (seq_t)n);
    seq_t Mpow = 1;           /* M^r, starts with r=0 -> 1 */
    seq_t ans = 0;

    /* Sum over r = 1 .. max_r */
    for (idx_t r = 1; r <= max_r; ++r) {
        /* Update Mpow = M^r */
        Mpow = Mpow * M;

        /* Compute m' = n - r(r-1)/2 */
        idx_t sub = (r * (r - 1)) / 2;
        if (sub > n) break;          /* m' would be negative */
        idx_t mprime = n - sub;

        /* p_exact = P_at_most[m'][r] - P_at_most[m'][r-1] */
        seq_t p_at_r   = P[(size_t)mprime * rsize + r];
        seq_t p_at_r1  = P[(size_t)mprime * rsize + (r - 1)];
        seq_t p_exact  = p_at_r - p_at_r1;
        if (!p_exact) continue;      /* skip if zero */

        /* ans += p_exact * Mpow */
        ans += p_exact * Mpow;
    }

    free_seq_array(P);
    return ans;
}
