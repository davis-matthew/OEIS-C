/*
 * A252720  Number of (1+2) X (n+2) 0..3 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + 6*a(n-3) - a(n-4) - 11*a(n-5) * - 12*a(n-6) +
 *            5*a(n-7) + 20*a(n-8) + 8*a(n-9) * - 8*a(n-10) - 12*a(n-11) +
 *            4*a(n-13) * for n > 14 * Base values: * a(1)=39, a(2)=52,
 *            a(3)=70, a(4)=96, a(5)=129, a(6)=175, * a(7)=237, a(8)=319,
 *            a(9)=432, a(10)=584, a(11)=786, * a(12)=1067, a(13)=14...
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 57, u64 n <= 118, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* memoisation cache */
static seq_t *cache      = NULL;        /* array a[0..max_cache] */
static size_t cache_cap  = 0;           /* allocated capacity in elements */
static idx_t  max_cache  = 0;           /* highest index stored */

/* base values (index 0 unused) */
static const seq_t base_vals[15] = {
    0,   /* a(0) unused */
    39, 52, 70, 96, 129, 175, 237, 319,
    432, 584, 786, 1067, 1442, 1947
};

/* initialise cache with capacity for at least 15 elements */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    cache_cap = 16;                  /* 0..15 */
    cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    if (!cache)
        abort();                     /* out of memory */

    for (idx_t i = 1; i <= 14; ++i)
        cache[i] = base_vals[i];

    max_cache = 14;
}

/* ensure capacity for indices 0..need */
static void grow_cache(idx_t need)
{
    if ((size_t)need + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)need + 1)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (!new_cache)
        abort();

    for (size_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];

    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    init_cache();

    if (n <= max_cache)
        return cache[n];

    /* compute missing terms up to n */
    grow_cache(n);

    for (idx_t i = max_cache + 1; i <= n; ++i)
    {
        seq_t term = cache[i - 2] * (seq_t)2;
        term += cache[i - 3] * (seq_t)6;
        term -= cache[i - 4];
        term -= cache[i - 5] * (seq_t)11;
        term -= cache[i - 6] * (seq_t)12;
        term += cache[i - 7] * (seq_t)5;
        term += cache[i - 8] * (seq_t)20;
        term += cache[i - 9] * (seq_t)8;
        term -= cache[i -10] * (seq_t)8;
        term -= cache[i -11] * (seq_t)12;
        term += cache[i -13] * (seq_t)4;
        cache[i] = term;
    }

    max_cache = n;
    return cache[n];
}
