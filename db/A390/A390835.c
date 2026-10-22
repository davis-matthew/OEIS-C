/*
 * A390835  Number of minimal total dominating sets in the n-necklace graph.
 * https://oeis.org/A390835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390835: Number of minimal total dominating sets in the n‑necklace graph
   Recurrence: a(n) = 6*a(n-1) - a(n-2) - 30*a(n-3) + 18*a(n-4) + 80*a(n-5)
               - 108*a(n-6) + 96*a(n-8) - 64*a(n-9)
   Initial values: a(0)=9, a(1)=6, a(2)=34, a(3)=108, a(4)=506, a(5)=2416,
                   a(6)=11194, a(7)=53584, a(8)=252674
   The function memoizes computed values to support calls in any order. */

#include <stdlib.h>

static const seq_t init_vals[9] = {
    (seq_t)9, (seq_t)6, (seq_t)34,
    (seq_t)108, (seq_t)506, (seq_t)2416,
    (seq_t)11194, (seq_t)53584, (seq_t)252674
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;

static seq_t compute_rec(idx_t i) {
    /* Compute a(i) for i >= 9 using the recurrence and the previously
       computed values stored in cache. The order of operations ensures
       that the intermediate result does not go negative. */
    seq_t res = cache[i - 1] * 6;                     /* 6*a(n-1) */
    res += cache[i - 4] * 18;                         /* +18*a(n-4) */
    res += cache[i - 5] * 80;                         /* +80*a(n-5) */
    res += cache[i - 8] * 96;                         /* +96*a(n-8) */
    res -= cache[i - 2];                              /* -a(n-2)   */
    res -= cache[i - 3] * 30;                         /* -30*a(n-3) */
    res -= cache[i - 6] * 108;                        /* -108*a(n-6)*/
    res -= cache[i - 9] * 64;                         /* -64*a(n-9) */
    return res;
}

seq_t a(idx_t n) {
    /* Handle the base cases directly. */
    if (n < 9) {
        return init_vals[n];
    }

    size_t needed = (size_t)(n + 1);

    /* First allocation (or grow if n exceeds current capacity). */
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (cache == NULL) {
            /* In a real system, handle out‑of‑memory more gracefully. */
            exit(1);
        }
        /* Copy the known initial values into the cache. */
        for (idx_t i = 0; i < 9; ++i) {
            cache[i] = init_vals[i];
        }
        /* Compute all values up to n consecutively. */
        for (idx_t i = 9; i <= n; ++i) {
            cache[i] = compute_rec(i);
        }
        cache_cap = needed;
    } else if (needed > cache_cap) {
        /* Grow the cache to accommodate the requested index. */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) {
            exit(1);
        }
        cache = tmp;
        /* Compute new values beyond the previous maximum. */
        for (idx_t i = cache_cap; i <= n; ++i) {
            cache[i] = compute_rec(i);
        }
        cache_cap = needed;
    }

    return cache[n];
}
