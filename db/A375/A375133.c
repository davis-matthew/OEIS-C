/*
 * A375133  Number of integer partitions of n whose maximal anti-runs have
 *          distinct maxima.
 * https://oeis.org/A375133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i>=0} (x^i * Product_{j=1..i-1} (1-x^(3*j))/(1-x^j))
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 177, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A375133 – number of integer partitions of n whose maximal
    anti‑runs have distinct maxima.  The routine below uses a simple
    dynamic‑programming scheme that counts the partitions directly
    (no hard‑coded table).  It is fast enough for n up to ~1e4.   */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
/*  The types seq_t and idx_t are supplied by the build.  */

seq_t a(idx_t n)
{
    /* Storage for the DP tables, grown on demand. */
    static idx_t capacity     = 0;      /* total allocated slots           */
    static seq_t *dp          = 0;      /* partitions of s with parts ≤ t */
    static seq_t *res         = 0;      /* final a(s) values               */
    static seq_t *tmp         = 0;      /* temporary copy of dp            */
    static idx_t max_n_done   = (idx_t)-1;   /* < 0 → not yet computed yet     */

    /*---( 1)  Grow buffers if needed  ---------------------------------*/
    idx_t need = n + 1;                 /* space for indices 0..n          */
    if (capacity < need) {
        idx_t new_cap = need * 2;       /* simple doubling policy          */
        if (new_cap < need) new_cap = need;
        dp  = (seq_t *)realloc(dp,  new_cap * sizeof(seq_t));
        res = (seq_t *)realloc(res, new_cap * sizeof(seq_t));
        tmp = (seq_t *)realloc(tmp, new_cap * sizeof(seq_t));
        if (!dp || !res || !tmp) exit(EXIT_FAILURE);
        capacity = new_cap;
    }

    /*---( 2)  Recompute the tables up to n  ---------------------------*/
    if (max_n_done == (idx_t)-1 || max_n_done < n) {
        idx_t N = n;

        /* initialise every entry to zero */
        for (idx_t i = 0; i <= N; ++i) {
            dp[i]  = 0;
            res[i] = 0;
        }
        dp[0]  = 1;      /* empty partition */
        res[0] = 1;      /* a(0) = 1        */

        /* iterate over allowed part sizes L = 1 .. N */
        for (idx_t L = 1; L <= N; ++L) {
            /* add the partitions whose largest part is L */
            for (idx_t s = L; s <= N; ++s)
                res[s] += dp[s - L];

            /* now update dp to allow one or two copies of L
               (no more than two because the construction forbids
                a part to appear more than twice in a partition
                with distinct maximal anti‑runs).           */
            for (idx_t i = 0; i <= N; ++i) tmp[i] = dp[i];  /* copy dp */
            for (idx_t s = L;         s <= N; ++s) dp[s] += tmp[s - L];
            for (idx_t s = 2 * L;     s <= N; ++s) dp[s] += tmp[s - 2 * L];
        }

        max_n_done = N;
    }

    return res[n];
}
