/*
 * A268783  Number of n X 2 binary arrays with some element plus some
 *          horizontally, vertically or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A268783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 2*a(n-1) + 3*a(n-2) - 2*a(n-3) -
 *            6*a(n-4) - 4*a(n-5) - a(n-6) Initial values: a(1) = 1, a(2) = 5,
 *            a(3) = 17, a(4) = 48, a(5) = 131, a(6) = 338
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 53, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* points to cached values a(0)…a(m) */
static size_t cache_len = 0;         /* number of cached entries (cache_len-1 is max cached index) */

static const seq_t base_vals[7] = {
    (seq_t)0,    /* a(0) unused, present only to keep indices aligned */
    (seq_t)1,    /* a(1) */
    (seq_t)5,    /* a(2) */
    (seq_t)17,   /* a(3) */
    (seq_t)48,   /* a(4) */
    (seq_t)131,  /* a(5) */
    (seq_t)338   /* a(6) */
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not part of the defined sequence */

    /* Ensure the cache is large enough */
    size_t needed = (size_t)(n + 1);           /* we want to store a(n) */
    if (needed > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();                     /* allocation failed */
        cache = tmp;

        /* initialise new entries */
        for (size_t i = cache_len; i < needed; ++i) {
            if (i <= 6) {
                cache[i] = base_vals[i];
            } else {
                /* recurrence: a_i = 2*a_{i-1} + 3*a_{i-2} - 2*a_{i-3} - 6*a_{i-4} - 4*a_{i-5} - a_{i-6} */
                seq_t pos = (seq_t)2 * cache[i-1] + (seq_t)3 * cache[i-2];
                seq_t neg = (seq_t)2 * cache[i-3] + (seq_t)6 * cache[i-4]
                            + (seq_t)4 * cache[i-5] + cache[i-6];
                cache[i] = pos - neg;          /* all values are non‑negative */
            }
        }
        cache_len = needed;
    }

    return cache[(size_t)n];
}
