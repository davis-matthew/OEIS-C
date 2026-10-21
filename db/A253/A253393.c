/*
 * A253393  Number of (n+1) X (4+1) 0..1 arrays with every 2 X 2 subblock
 *          antidiagonal maximum minus diagonal minimum nondecreasing
 *          horizontally and diagonal maximum minus antidiagonal minimum
 *          nondecreasing vertically.
 * https://oeis.org/A253393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 3*a(n-4) - a(n-5) (n >
 *            11) Initial values: a(1)=180, a(2)=197, a(3)=246, a(4)=346,
 *            a(5)=465, a(6)=632, a(7)=823, a(8)=1071, a(9)=1351, a(10)=1695,
 *            a(11)=2079
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* internal data */
static seq_t *cache = NULL;                /* array holding computed terms */
static idx_t  cache_cap = 0;               /* current capacity (index max + 1)   */
static idx_t  cache_last = 0;              /* highest index computed so far      */

/* base values for a(1) .. a(11) */
static const seq_t init_vals[12] = {
    0,      /* unused, 0-index */
    180,    /* a(1)  */
    197,    /* a(2)  */
    246,    /* a(3)  */
    346,    /* a(4)  */
    465,    /* a(5)  */
    632,    /* a(6)  */
    823,    /* a(7)  */
    1071,   /* a(8)  */
    1351,   /* a(9)  */
    1695,   /* a(10) */
    2079    /* a(11) */
};

/* ensure cache has space for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cache_cap >= n + 1)
        return;

    size_t new_cap = (cache_cap == 0) ? (size_t)(n + 1) : (size_t)cache_cap;
    while (new_cap < (size_t)(n + 1))
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
    {
        /* allocation failure – abort as the caller cannot handle it */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

/* initialise cache with the first 11 terms */
static void init_cache(void)
{
    if (cache != NULL)
        return;          /* already initialised */

    ensure_capacity(11);
    for (idx_t i = 1; i <= 11; ++i)
        cache[i] = init_vals[i];
    /* set unused entries to 0 */
    if (cache_cap > 12)
        for (idx_t i = 12; i < cache_cap; ++i)
            cache[i] = (seq_t)0;

    cache_last = 11;
}

/* compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* OEIS offset 1 */
    if (n == 0)
    {
        /* Undefined, but return 0 to avoid UB */
        return (seq_t)0;
    }

    init_cache();

    if (n <= cache_last)
        return cache[n];

    /* extend cache up to n */
    ensure_capacity(n);

    /* compute missing terms */
    for (idx_t i = cache_last + 1; i <= n; ++i)
    {
        if (i <= 11)
        {
            /* already set in init_cache, but protect against out‑of‑range */
            continue;
        }
        /* recurrence: a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 3*a(n-4) - a(n-5) */
        cache[i] =
            (seq_t)3 * cache[i - 1] -
            (seq_t)2 * cache[i - 2] -
            (seq_t)2 * cache[i - 3] +
            (seq_t)3 * cache[i - 4] -
            cache[i - 5];
    }

    cache_last = n;
    return cache[n];
}
