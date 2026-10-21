/*
 * A284179  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 813", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A284179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 10000*a(n-4) - 10000*a(n-5) (n ≥ 15) initial
 *            values (n = 0 … 14): 1, 10, 101, 1010, 10111, 101111, 1010111,
 *            10101011, 101111101, 1011111110, 10101111111, 101010111111,
 *            1011111011111, 10111111111111, 101011111111111
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic array of computed terms     */
static size_t cache_cap = 0;     /* current capacity of cache array     */
static idx_t cache_size = 0;     /* number of terms stored (max index+1) */

/* Ensure the cache can hold values up to index 'n' (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)(n + 1))
        new_cap *= 2;

#if defined(__cplusplus)
    seq_t *new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
#else
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the cache with the first 15 terms. */
static void init_cache(void)
{
    if (cache)
        return;

    cache_cap = 16;
#if defined(__cplusplus)
    cache = new seq_t[cache_cap];
#else
    cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
#endif
    const seq_t init[15] = {
        1ULL,              /* a(0) */
        10ULL,             /* a(1) */
        101ULL,            /* a(2) */
        1010ULL,           /* a(3) */
        10111ULL,          /* a(4) */
        101111ULL,         /* a(5) */
        1010111ULL,        /* a(6) */
        10101011ULL,       /* a(7) */
        101111101ULL,      /* a(8) */
        1011111110ULL,     /* a(9) */
        10101111111ULL,    /* a(10) */
        101010111111ULL,   /* a(11) */
        1011111011111ULL,  /* a(12) */
        10111111111111ULL, /* a(13) */
        101011111111111ULL/* a(14) */
    };
    for (idx_t i = 0; i < 15; ++i)
        cache[i] = init[i];
    cache_size = 15;
}

seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_size)
        return cache[n];

    ensure_capacity(n);

    for (idx_t i = cache_size; i <= n; ++i) {
        /* recurrence applies for i ≥ 15, a(i-4) and a(i-5) are valid */
        seq_t diff = cache[i - 4] - cache[i - 5];
        cache[i] = cache[i - 1] + (seq_t)10000 * diff;
    }

    cache_size = n + 1;
    return cache[n];
}
