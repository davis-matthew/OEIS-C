/*
 * A267158  Number of ON (black) cells in the n-th iteration of the "Rule 107"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-4) - a(n-6) with initial values: a(0) = 1,
 *            a(1) = 1, a(2) = 3, a(3) = 5, a(4) = 6, a(5) = 8, a(6) = 7, a(7)
 *            = 13, a(8) = 6, a(9) = 15, a(10) = 8, a(11) = 21, a(12) = 6
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* static state for memoisation */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;
static idx_t  computed_max = 12;   /* last index whose value is known */
static int    base_set   = 0;      /* true when base values are copied */

/* base values a(0)..a(12) */
static const seq_t base_vals[13] = {
    (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)5, (seq_t)6,
    (seq_t)8, (seq_t)7, (seq_t)13, (seq_t)6, (seq_t)15,
    (seq_t)8, (seq_t)21, (seq_t)6
};

/* ensure cache can address index n and copy base values once */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;

    size_t new_size = cache_size ? cache_size : 16;
    while (new_size <= (size_t)n)
        new_size <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort();          /* allocation failed */

    if (!cache) {
        /* first allocation: zero initialise (optional) */
        memset(new_cache, 0, new_size * sizeof(seq_t));
    }
    cache   = new_cache;
    cache_size = new_size;

    if (!base_set) {
        /* copy the known initial terms into the cache */
        memcpy(cache, base_vals, sizeof(base_vals));
        base_set = 1;
        computed_max = 12;
    }
}

seq_t a(idx_t n)
{
    if (n < 13)
        return base_vals[n];

    ensure_cache(n);

    /* compute any missing terms up to n */
    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            cache[i] = cache[i-2] + cache[i-4] - cache[i-6];
        }
        computed_max = n;
    }

    return cache[n];
}
