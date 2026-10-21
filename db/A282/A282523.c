/*
 * A282523  Number of n X 3 0..1 arrays with no 1 equal to more than two of
 *          its king-move neighbors.
 * https://oeis.org/A282523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A282523: Number of n X 3 0..1 arrays with no 1 equal to more than two
   of its king-move neighbors. Empirical recurrence:
   a(n) = 5*a(n‑1) + 2*a(n‑2) - a(n‑3) - 24*a(n‑4) + 13*a(n‑5)
          - 2*a(n‑7) + 6*a(n‑8)
   Initial values (a(1) … a(8)):
   a(1)=8   a(2)=43   a(3)=206   a(4)=1097   a(5)=5675
   a(6)=29433   a(7)=153037   a(8)=794716 */

#include <stddef.h>
#include <stdlib.h>

/* Cached sequence values.  cache[0] is unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Initialise the cache with the first 8 values. */
static void init_cache(void)
{
    if (cache_cap >= 9) return;            /* already initialised */

    cache_cap = 9;                         /* indices 0..8 inclusive */
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!cache) return;                    /* allocation failure – unlikely */

    cache[1] = (seq_t)8;
    cache[2] = (seq_t)43;
    cache[3] = (seq_t)206;
    cache[4] = (seq_t)1097;
    cache[5] = (seq_t)5675;
    cache[6] = (seq_t)29433;
    cache[7] = (seq_t)153037;
    cache[8] = (seq_t)794716;
}

/* Return the n-th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* a(0) is undefined */

    init_cache();

    if ((size_t)n < cache_cap)              /* already computed */
        return cache[n];

    /* Extend the cache up to and including n. */
    size_t new_cap = (size_t)n + 1;          /* 0 index unused */
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute successive terms using the recurrence. */
    for (size_t i = 9; i <= (size_t)n; ++i) {
        cache[i] = 5 * cache[i - 1] + 2 * cache[i - 2]
                 - cache[i - 3] - 24 * cache[i - 4] + 13 * cache[i - 5]
                 - 2 * cache[i - 7] + 6 * cache[i - 8];
    }

    return cache[n];
}
