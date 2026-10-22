/*
 * A308638  a(n) is the number of k > 0 such that n-1-2*k > 0 and the points
 *          (n-1-2*k, a(n-1-2*k)), (n-1-k, a(n-1-k)) and (n-1, a(n-1)) are
 *          aligned.
 * https://oeis.org/A308638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = |{ k | 1 ≤ k ≤ floor((n-2)/2) and * a(n-1-2k) + a(n-1) =
 *            2·a(n-1-k) }|, * with a(1) = a(2) = 0. The expression follows
 *            from the * collinearity condition of points (n-1-2k, a(n-1-2k)),
 *            * (n-1-k, a(n-1-k)) and (n-1, a(n-1))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* protective guard; offset starts at 1 */

    static seq_t *cache = NULL;   /* cache[0] holds a(1) */
    static idx_t cached_n = 0;    /* number of cached entries */

    /* lazy allocation of the cache */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure */
        cache[0] = 0;                  /* a(1) */
        cached_n = 1;
    }

    /* enlarge cache if necessary */
    if (cached_n < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (tmp == NULL) return cache[n-1]; /* fall back; should not happen */
        cache = tmp;

        /* compute new entries */
        for (idx_t i = cached_n; i < n; ++i) {
            if (i == 0 || i == 1) {   /* a(1) and a(2) */
                cache[i] = 0;
                continue;
            }
            /* i is zero‑based index of a(n) */
            idx_t m = i - 1;          /* zero‑based index of a(n-1) */
            seq_t count = 0;
            idx_t limit = m / 2;      /* floor((n-2)/2) in zero‑based terms */
            for (idx_t k = 1; k <= limit; ++k) {
                idx_t pos1 = m - 2 * k;   /* zero‑based index of a(n-1-2k) */
                idx_t pos2 = m - k;       /* zero‑based index of a(n-1-k)   */
                /* collinearity condition: a(n-1-2k) + a(n-1) == 2·a(n-1-k) */
                if (cache[pos1] + cache[m] == (seq_t)2 * cache[pos2])
                    ++count;
            }
            cache[i] = count;
        }
        cached_n = n;
    }

    return cache[n - 1];
}
