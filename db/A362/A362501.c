/*
 * A362501  Number of vertex cuts in the n-alkane graph.
 * https://oeis.org/A362501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 14*a(n-1) - 57*a(n-2) + 76*a(n-3) - 32*a(n-4) for n>=5
   Initial values: a(1)=11, a(2)=169, a(3)=1699, a(4)=14989 */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 1; /* next unused index; initially 1 (no valid data) */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Off‑set is 1, value for n=0 is undefined */

    /* First call: allocate and initialize */
    if (cache == NULL) {
        /* Allocate enough room for the first few terms and grow as needed */
        size_t init_cap = (n < 16) ? 16 : n + 1;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE); /* allocation failed */

        /* Set initial terms */
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)169;
        cache[3] = (seq_t)1699;
        cache[4] = (seq_t)14989;

        cache_cap = init_cap;
        cache_len = 5; /* next free index (one past highest stored term) */
    }

    /* Grow the cache if the requested n exceeds current capacity */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE); /* realloc failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n >= cache_len) {
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t val = 0;
            if (k >= 5) {
                /* a(k) = 14*a(k-1) - 57*a(k-2) + 76*a(k-3) - 32*a(k-4) */
                val = cache[k-1] * (seq_t)14
                        - cache[k-2] * (seq_t)57
                        + cache[k-3] * (seq_t)76
                        - cache[k-4] * (seq_t)32;
            }
            cache[k] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
