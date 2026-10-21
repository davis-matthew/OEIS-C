/*
 * A281201  Number of n X 4 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 4*a(n-3) - 6*a(n-4) + 2*a(n-5) +
 *            4*a(n-6) - a(n-8) Initial values (n>=1): a(1)=2, a(2)=28,
 *            a(3)=98, a(4)=270, a(5)=676, a(6)=1588, a(7)=3604, a(8)=7960
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 26, u64 n <= 59, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[n] holds a(n), 1‑based index */
static idx_t   computed_max = 0;     /* largest n for which cache[n] is valid */
static size_t  cache_alloc = 0;      /* currently allocated size */

/* Initialize the cache with the first 8 terms (index 0 unused) */
static void init_cache(void) {
    if (cache != NULL) {
        free(cache);
    }
    /* Allocate space for indices 0..8 (9 entries) */
    cache = (seq_t *)malloc(9 * sizeof(seq_t));
    if (cache == NULL) return; /* Allocation failed: caller will fail gracefully */

    cache_alloc = 9;
    cache[0] = (seq_t)0;          /* unused sentinel */
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)28;
    cache[3] = (seq_t)98;
    cache[4] = (seq_t)270;
    cache[5] = (seq_t)676;
    cache[6] = (seq_t)1588;
    cache[7] = (seq_t)3604;
    cache[8] = (seq_t)7960;
    computed_max = 8;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset is 1; n==0 is not used but safe */

    /* Ensure cache has been initialized */
    if (computed_max == 0) {
        init_cache();
        if (cache == NULL) return (seq_t)0; /* allocation failed */
    }

    /* Return if already computed */
    if (n <= computed_max) {
        return cache[n];
    }

    /* Grow the cache if necessary */
    if ((size_t)(n + 1) > cache_alloc) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            free(cache);
            cache = NULL;
            cache_alloc = 0;
            return (seq_t)0; /* Allocation failed */
        }
        cache = tmp;
        cache_alloc = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* All indices i-1 .. i-8 are guaranteed to be valid because i >= 9 */
        seq_t pos = (seq_t)2 * cache[i-1] + (seq_t)3 * cache[i-2]
                    + (seq_t)2 * cache[i-5] + (seq_t)4 * cache[i-6];
        seq_t neg = (seq_t)4 * cache[i-3] + (seq_t)6 * cache[i-4] + cache[i-8];
        cache[i] = pos - neg;
    }
    computed_max = n;
    return cache[n];
}
