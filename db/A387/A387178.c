/*
 * A387178  Number of integer partitions of n whose parts have choosable sets
 *          of strict integer partitions.
 * https://oeis.org/A387178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A387178: Number of integer partitions of n whose parts have
   choosable sets of strict integer partitions.  Equivalently,
   partitions in which each part k occurs at most A000009(k) times,
   where A000009(k) is the number of partitions of k into distinct
   parts.  This implementation computes A000009(k) for all k≤n,
   then performs a bounded coin‑change DP with multiplicities
   Lk=min(A000009(k),⌊n/k⌋).  The algorithm runs in O(n²) time
   and uses O(n) memory, handling n≈10000 in well under two
   seconds.  The result is exact whenever it fits in seq_t. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* a(n) ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* special case: empty partition */
    if (n == 0) return (seq_t)1;

    /* ---------- 1: compute A000009(k) for all k≤n, capped at n+1 ---------- */
    /* dist[s] holds min(A000009(s), n+1).  idx_t is at least 64‑bit.     */
    idx_t *dist = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!dist) return (seq_t)0;          /* allocation failure */
    memset(dist, 0, (n + 1) * sizeof(idx_t));
    dist[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t s = n; s >= i; --s) {
            idx_t add = dist[s - i];
            idx_t sum = dist[s] + add;
            if (sum > n + 1) sum = n + 1;
            dist[s] = sum;
        }
    }

    /* ---------- 2: bounded coin‑change DP -------------------------------- */
    seq_t *dp_prev = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *dp_cur  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp_prev || !dp_cur) {
        free(dist);
        free(dp_prev);
        free(dp_cur);
        return (seq_t)0;
    }
    dp_prev[0] = (seq_t)1;   /* base: one way to form 0 */

    for (idx_t k = 1; k <= n; ++k) {
        /* multiplicity limit for part k */
        idx_t limit = n / k;
        idx_t Lk    = dist[k];
        if (Lk > limit) Lk = limit;      /* clamp to n/k */
        idx_t window = Lk + 1;            /* number of terms in sliding window */

        /* process each residue class modulo k */
        for (idx_t r = 0; r < k; ++r) {
            seq_t sum = (seq_t)0;
            idx_t t = 0;   /* position inside current window */
            for (idx_t s = r; s <= n; s += k, ++t) {
                sum += dp_prev[s];
                if (t > Lk) {   /* remove element that falls out of window */
                    sum -= dp_prev[s - window * k];
                }
                dp_cur[s] = sum;
            }
        }

        /* swap dp_prev and dp_cur for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur  = tmp;
        /* dp_cur will be overwritten in the next loop */
    }

    seq_t result = dp_prev[n];

    /* clean up */
    free(dist);
    free(dp_prev);
    free(dp_cur);

    return result;
}
