/*
 * A362362  Number of permutations of [n] such that each cycle contains its
 *          length as an element.
 * https://oeis.org/A362362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A362362 (Number of permutations of [n] such that each cycle contains its length as an element)
 *
 *  a(n) = Σ_{k=1}^{kmax} (n-k)! · P(n - k·(k-1)/2 , k)
 *
 *  where P(m,k) is the number of partitions of m into exactly k (unordered) positive parts
 *  and kmax = ⌊(√(8n+1)-1)/2⌋ is the maximal number of distinct parts that can sum to n.
 *
 *  The recurrence for P(m,k) (unrestricted partitions) is
 *      P(m,k) = P(m-1,k-1) + P(m-k,k)    (m,k ≥ 1),   P(0,0)=1, else 0.
 *
 *  This implementation builds a cache of factorials and P(m,k) up to the largest n requested,
 *  and recomputes the tables only when a larger n occurs.  All arithmetic is performed in the
 *  seq_t type, which may be an 8/16/32/64‑bit unsigned integer, an unsigned __int128,
 *  or an arbitrary‑precision integer class.
 */

#include <stdlib.h>
#include <math.h>

static inline idx_t kmax_for_n(idx_t n)
{
    double s = sqrt((double)n * 8.0 + 1.0);
    return (idx_t)((s - 1.0) / 2.0);
}

static void recompute(idx_t N, idx_t kmax,
                      seq_t **fact_ptr, seq_t **dp_ptr)
{
    // Allocate factorial table
#ifdef __cplusplus
    seq_t *new_fact = new seq_t[N + 1];
    seq_t *new_dp   = new seq_t[(N + 1) * (kmax + 1)];
#else
    seq_t *new_fact = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *new_dp   = (seq_t *)malloc(((N + 1) * (kmax + 1)) * sizeof(seq_t));
    if (!new_fact || !new_dp) { /* memory error – abort */ return; }
#endif

    /* initialize factorials */
    new_fact[0] = 1;
    for (idx_t i = 1; i <= N; ++i)
        new_fact[i] = new_fact[i - 1] * (seq_t)i;

    /* initialize DP table to zeros */
    size_t total = (size_t)(N + 1) * (size_t)(kmax + 1);
    for (size_t i = 0; i < total; ++i) new_dp[i] = 0;

    /* base case */
    new_dp[0 * (kmax + 1) + 0] = 1;

    /* fill DP table for unrestricted partitions */
    for (idx_t n = 1; n <= N; ++n) {
        for (idx_t k = 1; k <= kmax; ++k) {
            seq_t val = new_dp[(n - 1) * (kmax + 1) + (k - 1)];
            if (n >= k) val += new_dp[(n - k) * (kmax + 1) + k];
            new_dp[n * (kmax + 1) + k] = val;
        }
    }

    /* free old arrays if any */
#ifdef __cplusplus
    if (*fact_ptr) delete[] *fact_ptr;
    if (*dp_ptr)   delete[] *dp_ptr;
#else
    free(*fact_ptr);
    free(*dp_ptr);
#endif

    *fact_ptr = new_fact;
    *dp_ptr   = new_dp;
}

/* ----------------- exported function ----------------- */

seq_t a(idx_t n)
{
    /* static cache variables */
    static idx_t cached_N = 0;
    static idx_t cached_kmax = 0;
    static seq_t *fact = NULL;
    static seq_t *dp   = NULL;

    if (n == 0) return (seq_t)1;

    idx_t req_kmax = kmax_for_n(n);
    if (n > cached_N) {
        recompute(n, req_kmax, &fact, &dp);
        cached_N = n;
        cached_kmax = req_kmax;
    } else if (req_kmax > cached_kmax) {
        /* if we need a larger kmax but same N, recompute with larger kmax */
        recompute(cached_N, req_kmax, &fact, &dp);
        cached_kmax = req_kmax;
    }

    seq_t ans = 0;
    for (idx_t k = 1; k <= req_kmax; ++k) {
        idx_t m = n - (k * (k - 1)) / 2;
        if (m < k) continue;           /* no partitions of m into k parts */
        seq_t ways = dp[m * (cached_kmax + 1) + k];
        if (ways == 0) continue;
        seq_t term = ways * fact[n - k];
        ans += term;
    }
    return ans;
}
