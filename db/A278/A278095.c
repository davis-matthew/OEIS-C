/*
 * A278095  Number of n X 2 0..3 arrays with every element plus 1 mod 4 equal
 *          to some element at offset (-1,0) (-1,1) (0,-1) (0,1) or (1,0),
 *          with upper left element zero.
 * https://oeis.org/A278095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A278095 via recurrence * a(1) = 0, a(2) = 2, a(3) = 10, a(4) =
 *            58, a(5) = 334, * a(6) = 1910, a(7) = 10910, * a(n) = 6*a(n-1) -
 *            a(n-2) - 3*a(n-3) - 3*a(n-4) * - 6*a(n-5) + 2*a(n-7) for n ≥ 8
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* 1‑based cache of computed values */
static size_t cache_cap = 0;      /* number of slots allocated (incl. 0) */
static idx_t  cache_max = 0;      /* largest n that has been computed */

/* Ensure the cache can hold indices up to at least 'n' */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* out of memory – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the base values 1…7 */
static void init_cache(void)
{
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)10;
    cache[4] = (seq_t)58;
    cache[5] = (seq_t)334;
    cache[6] = (seq_t)1910;
    cache[7] = (seq_t)10910;
    cache_max = 7;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for 0, but return 0 */

    /* First call: allocate cache and set seed values */
    if (!cache)
    {
        ensure_capacity(n);           /* at least n slots needed */
        /* Allocate memory large enough for indices 0…n */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failed */
        init_cache();
    }

    if (n > cache_max)               /* compute missing values up to n */
    {
        if ((size_t)n + 1 > cache_cap)
            ensure_capacity(n);

        for (idx_t i = cache_max + 1; i <= n; ++i)
        {
            /* Recurrence is defined for i ≥ 8 */
            seq_t val = 6 * cache[i-1]
                       - cache[i-2]
                       - 3 * cache[i-3]
                       - 3 * cache[i-4]
                       - 6 * cache[i-5]
                       + 2 * cache[i-7];
            cache[i] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
