/*
 * A296957  Number of n X 2 0..1 arrays with each 1 adjacent to 2, 3 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296957
 *
 * Recurrence:   a(n) = 4*a(n-1) - 5*a(n-2) + 11*a(n-3) - 18*a(n-4) + 4*a(n-5)
 * Initial values: a(1)=1, a(2)=6, a(3)=16, a(4)=31, a(5)=96
 * Offset: 1
 *
 * The function caches computed values so that calls may arrive in any order.
 * It works for any seq_t (built‑in or arbitrary‑precision) as long as the
 * arithmetic operators are defined for that type.
 */

#include <stdlib.h>

/* cached results */
static seq_t *cache = NULL;
static size_t cache_cap = 0;          /* number of allocated slots (>= n+1) */
static idx_t  cache_last = 0;         /* largest n whose value is already computed */

/* ensure that base values a(1)..a(5) are present in the cache */
static void init_cache(void)
{
    if (!cache)
    {
        cache_cap = 6;                           /* indices 0..5 are available */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        /* base values (a[0] unused) */
        static const seq_t base[6] = {0, 1, 6, 16, 31, 96};
        for (idx_t i = 1; i <= 5; ++i)
            cache[i] = base[i];
        cache_last = 5;
    }
}

/* ensure cache has space for at least indices 0..n */
static void ensure_capacity(idx_t n)
{
    if (n + 1 > cache_cap)
    {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)        /* allocation failed – abort */
            abort();
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1 – return 0 for n=0 */
        return (seq_t)0;

    init_cache();          /* make sure base values are in place */
    ensure_capacity(n);    /* guarantee we can store up to n */

    if (n <= cache_last)  /* already computed */
        return cache[n];

    /* compute values from cache_last + 1 up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i)
    {
        seq_t t1 = (seq_t)4 * cache[i-1];
        seq_t t2 = (seq_t)5 * cache[i-2];
        seq_t t3 = (seq_t)11 * cache[i-3];
        seq_t t4 = (seq_t)18 * cache[i-4];
        seq_t t5 = (seq_t)4 * cache[i-5];

        /* a(i) = t1 - t2 + t3 - t4 + t5   (performed as sum of positives minus sum of negatives) */
        seq_t pos = t1 + t3 + t5;
        seq_t neg = t2 + t4;
        cache[i] = pos - neg;   /* result is guaranteed non‑negative */
    }
    cache_last = n;

    return cache[n];
}
