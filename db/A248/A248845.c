/*
 * A248845  Triangle read by rows: T(m,n) = number of ways of distributing n
 *          distinguishable balls into m distinguishable bins of size 3 where
 *          empty bins are permitted (m >= 1, 1 <= n <= 3m).
 * https://oeis.org/A248845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the number of ways to distribute k distinguishable balls
 *            into m distinguishable * bins each of size 3, where (m,k) is
 *            obtained by reading the triangle of T(m,k) = * Σ_{k1+...+km=n,
 *            0≤ki≤3} n!/(k1!…km!) row by row
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function assumes the types seq_t and idx_t are defined by the build
 * environment. It does not use any external helpers beyond standard headers
 * and a simple binomial routine defined below. */

#include <stdlib.h>
#include <stdint.h>

/* Small helper: binomial coefficient C(n,k) for non‑negative n,k. */
static inline seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply by (n - k + i) then divide by i */
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

/* Given an overall index n (1‑based), determine the row m and column k
 * in the triangle. */
static void index_to_rc(idx_t n, idx_t *m_out, idx_t *k_out)
{
    /* Find minimal m such that 3*m*(m+1)/2 >= n. */
    idx_t low = 1, high = 1;
    while ( ((__int128)3 * high * (high + 1)) / 2 < n ) high *= 2;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ( ((__int128)3 * mid * (mid + 1)) / 2 >= n )
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;
    idx_t start = ((__int128)3 * (m - 1) * m) / 2 + 1;
    idx_t k    = n - start + 1;      /* 1‑based within the row */
    *m_out = m;
    *k_out = k;
}

/* Compute T(m,k): number of ways to distribute k balls into m bins of size 3. */
static seq_t T_of_m_k(idx_t m, idx_t k)
{
    if (k > 3 * m) return (seq_t)0;
    if (m == 0) return (k == 0) ? (seq_t)1 : (seq_t)0;

    /* DP array of size k+1, initialized to 0. */
    seq_t *dp = (seq_t *)malloc((size_t)(k + 1) * sizeof(seq_t));
    seq_t *dp_next = (seq_t *)malloc((size_t)(k + 1) * sizeof(seq_t));
    if (!dp || !dp_next) {
        /* Allocation failure; return 0 (should not happen for small k). */
        free(dp); free(dp_next);
        return (seq_t)0;
    }
    for (idx_t i = 0; i <= k; ++i) dp[i] = dp_next[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    for (idx_t bin = 0; bin < m; ++bin) {
        /* Reset dp_next to zeros */
        for (idx_t i = 0; i <= k; ++i) dp_next[i] = (seq_t)0;
        for (idx_t j = 0; j <= k; ++j) {
            if (dp[j] == (seq_t)0) continue;
            for (idx_t add = 0; add <= 3; ++add) {
                idx_t nv = j + add;
                if (nv > k) break;
                seq_t ways = binom_idx(k - j, add);
                dp_next[nv] = dp_next[nv] + dp[j] * ways;
            }
        }
        /* Swap dp and dp_next */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    seq_t result = dp[k];
    free(dp); free(dp_next);
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; just return 0 */
    idx_t m, k;
    index_to_rc(n, &m, &k);
    return T_of_m_k(m, k);
}
