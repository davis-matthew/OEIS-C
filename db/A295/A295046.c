/*
 * A295046  Number of n X 3 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0 or 2 1s.
 * https://oeis.org/A295046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) - 3*a(n-3) - 2*a(n-4) - 6*a(n-5) +
 *            3*a(n-6) - a(n-7) + a(n-9) for n >= 10 Initial values: a(1) = 5,
 *            a(2) = 19, a(3) = 72, a(4) = 270, a(5) = 1027, a(6) = 3879, a(7)
 *            = 14691, a(8) = 55589, a(9) = 210418
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, calloc, realloc */

/* External types supplied by the build system */
extern seq_t a(idx_t n);

static const seq_t init_terms[9] = {
    5ULL,    19ULL,    72ULL,    270ULL,
    1027ULL, 3879ULL, 14691ULL, 55589ULL,
    210418ULL
};

static seq_t *cache = NULL;      /* cache[0] = a(1), cache[1] = a(2), ...        */
static size_t cache_cap = 0;      /* allocated size of cache (in elements)    */
static idx_t computed_up_to = 0;  /* highest n for which cache contains a(n) */

static void ensure_cache_size(idx_t n)
{
    if (cache == NULL) {
        cache_cap = (size_t)(n < 9 ? 9 : n);
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();          /* out of memory */
    } else if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1; behaviour below 1 not required */

    ensure_cache_size(n);

    if (n <= computed_up_to) {
        return cache[(size_t)(n - 1)];
    }

    /* Compute new terms up to n */
    for (idx_t k = computed_up_to + 1; k <= n; ++k) {
        if (k <= 9) {          /* copy known initial values */
            cache[(size_t)(k - 1)] = init_terms[(size_t)(k - 1)];
            continue;
        }

        /* recurrence: a(k) = 3*a(k-1) + 4*a(k-2) - 3*a(k-3) - 2*a(k-4) - 6*a(k-5)
                        + 3*a(k-6) - a(k-7) + a(k-9) */
        seq_t val = 3 * cache[(size_t)(k - 2)] +          /* a(k-1)  */
                    4 * cache[(size_t)(k - 3)] -          /* a(k-2)  */
                   3 * cache[(size_t)(k - 4)] -          /* a(k-3)  */
                   2 * cache[(size_t)(k - 5)] -          /* a(k-4)  */
                   6 * cache[(size_t)(k - 6)] +          /* a(k-5)  */
                   3 * cache[(size_t)(k - 7)] -          /* a(k-6)  */
                   1 * cache[(size_t)(k - 8)] +          /* a(k-7)  */
                   1 * cache[(size_t)(k -10)];           /* a(k-9)  */

        cache[(size_t)(k - 1)] = val;
    }

    computed_up_to = n;
    return cache[(size_t)(n - 1)];
}
