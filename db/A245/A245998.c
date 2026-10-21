/*
 * A245998  Number of length 3+2 0..n arrays with no pair in any consecutive
 *          three terms totalling exactly n.
 * https://oeis.org/A245998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 8*a(n-3) + 6*a(n-4) + 6*a(n-5) - 8*a(n-6) +
 *            3*a(n-8) - a(n-9) Initial values: a(1)=2, a(2)=18, a(3)=164,
 *            a(4)=676, a(5)=2262, a(6)=5766, a(7)=13064, a(8)=25992,
 *            a(9)=48170
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* 1‑based cache, cache[0] unused */
static idx_t  cache_size = 0;    /* highest index computed */
static size_t cache_cap   = 0;   /* allocated capacity (bytes) */

/* Ensure that the cache can hold at least n+1 entries (indexing from 0). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failure – abort (should not happen in tests) */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; zero is unused */
    if (n <= cache_size) return cache[n];

    ensure_cache(n + 1);   /* ensure space for index n */

    /* Initialize first nine values if not already done */
    if (cache_size < 9) {
        static const seq_t init[10] = {0,
            2, 18, 164, 676, 2262, 5766, 13064, 25992, 48170};
        for (idx_t i = cache_size + 1; i <= 9 && i <= n; ++i) {
            cache[i] = init[i];
        }
        cache_size = 9;
    }

    /* Compute subsequent terms */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 9) {
            /* already handled by the init block */
            continue;
        }
        /* Positive contributions */
        seq_t t1 = cache[i-1] * (seq_t)3;   /* 3*a(i-1) */
        seq_t t2 = cache[i-4] * (seq_t)6;   /* 6*a(i-4) */
        seq_t t3 = cache[i-5] * (seq_t)6;   /* 6*a(i-5) */
        seq_t t4 = cache[i-8] * (seq_t)3;   /* 3*a(i-8) */

        seq_t total = t1 + t2 + t3 + t4;

        /* Negative contributions */
        seq_t s1 = cache[i-3] * (seq_t)8;   /* 8*a(i-3) */
        seq_t s2 = cache[i-6] * (seq_t)8;   /* 8*a(i-6) */
        seq_t s3 = cache[i-9];              /*  a(i-9)  */

        seq_t sub = s1 + s2 + s3;

        /* Since total > sub for all valid i, subtraction is safe */
        cache[i] = total - sub;
    }
    cache_size = n;
    return cache[n];
}
