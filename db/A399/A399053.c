/*
 * A399053  Sum of cubefree numbers <= n.
 * https://oeis.org/A399053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} i * A212793(i) (A212793(i)=1 iff i is
 *            cubefree)
 * Range:     u8 n <= 24, u16 n <= 397, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined for n ≥ 1, but return 0 for safety. */
        return (seq_t)0;
    }

    /* Persistent cache for cumulative sums a(0) .. a(max_n_cached). */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t max_n_cached = 0;

    /* Persistent smallest‑prime‑factor table up to spf_limit. */
    static idx_t *spf = NULL;
    static idx_t spf_limit = 0;

    /* Allocate minimal cache if it does not exist yet. */
    if (cache_cap == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache_cap = 1;
        cache[0] = (seq_t)0;        /* a(0) = 0 for the recurrence */
    }

    /* Grow the cache array if needed. */
    if (cache_cap < n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = n + 1;
    }

    /* Ensure a largest‑prime‑factor table that covers the required range. */
    if (spf_limit < n) {
        if (spf) free(spf);
        build_spf(n, &spf);          /* builds spf[0..n] */
        spf_limit = n;
    }

    /* Compute incremental values up to n. */
    for (idx_t i = max_n_cached + 1; i <= n; ++i) {
        bool cubefree = true;
        idx_t x = i;
        while (x > 1) {
            idx_t p = spf[x];
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
                if (cnt >= 3) {
                    cubefree = false;
                    break;
                }
            }
            if (!cubefree) break;
        }

        cache[i] = cache[i - 1];
        if (cubefree) {
            cache[i] = cache[i] + (seq_t)i;
        }
    }

    max_n_cached = n;
    return cache[n];
}
