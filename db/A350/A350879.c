/*
 * A350879  Triangle T(n,k), n >= 1, 1 <= k <= n, read by rows, where T(n,k)
 *          is the number of partitions of n such that k*(greatest part) =
 *          (number of parts).
 * https://oeis.org/A350879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 406, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A350879: a(n) is the n-th entry of the triangle T(n,k) read
 * row‑by‑row, where T(n,k) counts partitions of n with greatest part 
 * = k·(#parts).  
 * The implementation maps index n → (row r, column k) and counts
 * partitions using a 3‑dimensional DP for p(n,t,m) = #partitions of n
 * into t parts with maximal part ≤ m.  Then  
 *   T(r,k) = Σ_{t=1}^{⌊r/k⌋} (p(r,t,t·k) – p(r,t,t·k−1)). 
 * The DP is precomputed once for all n≤200, which is sufficient for
 * all requested indices up to n=10 000 (rows ≤141).
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"   /* seq_t, idx_t, alloc_seq_array */

static const idx_t G_MAX = 200;          /* sufficient for n ≤ 10 000 */
static seq_t *g_dp = NULL;               /* flattened 3‑D array */

static inline seq_t *dp_at(idx_t m, idx_t t, idx_t n)
{
    /* index = ((m*(G_MAX+1)+t)*(G_MAX+1)+n) */
    return &g_dp[((m * (G_MAX + 1) + t) * (G_MAX + 1)) + n];
}

static void init_dp(void)
{
    if (g_dp != NULL)
        return;                          /* already initialized */

    /* total elements = (G_MAX+1)^3 */
    size_t total = (size_t)(G_MAX + 1) * (G_MAX + 1) * (G_MAX + 1);
    g_dp = alloc_seq_array(total);
    if (g_dp == NULL) {
        /* allocation failed – abort execution (caller will handle it) */
        return;
    }

    /* base case: dp[0][0][m] = 1 for all m, dp[n>0][t>0][0] = 0 (array is zero‑init) */
    g_dp[0] = 1;                          /* dp[0][0][0] */

    /* compute DP */
    for (idx_t m = 1; m <= G_MAX; ++m) {
        for (idx_t t = 0; t <= G_MAX; ++t) {
            for (idx_t n = 0; n <= G_MAX; ++n) {
                seq_t val = *dp_at(m - 1, t, n);   /* dp[n][t][m-1] */
                if (t > 0 && n >= m) {
                    val += *dp_at(m, t - 1, n - m); /* dp[n-m][t-1][m] */
                }
                *dp_at(m, t, n) = val;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset = 1, a(1) is first term; below offset is irrelevant */
        return 0;

    /* 1. map index n to (row r, column k) of the triangle */
    idx_t r, k;

    /* approximate r via sqrt, adjust to guarantee r(r+1)/2 ≥ n */
    double t = sqrt((double)8 * n - 7);
    r = (idx_t)((t - 1.0) / 2.0);
    while (r * (r + 1) / 2 < n)
        ++r;
    while (r * (r - 1) / 2 >= n)
        --r;
    k = n - r * (r - 1) / 2;      /* 1 ≤ k ≤ r */

    /* 2. ensure DP table is ready */
    init_dp();
    if (g_dp == NULL)           /* allocation failed */
        return 0;

    /* 3. compute T(r,k) via DP */
    seq_t result = 0;
    idx_t tmax = r / k;          /* maximum number of parts */
    for (idx_t tcnt = 1; tcnt <= tmax; ++tcnt) {
        idx_t m = tcnt * k;      /* largest part in such partitions */
        seq_t high = *dp_at(m, tcnt, r);          /* p(r, tcnt, m)   */
        seq_t low  = *dp_at(m - 1, tcnt, r);     /* p(r, tcnt, m-1) */
        result += high - low;
    }
    return result;
}
