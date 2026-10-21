/*
 * A243602  Number of length n+2 0..5 arrays with no three elements in a row
 *          with pattern aba (with a!=b) and new values 0..5 introduced in
 *          0..5 order.
 * https://oeis.org/A243602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 48*a(n-2) + 65*a(n-3) + 18*a(n-4) * -
 *            74*a(n-5) - 9*a(n-6) + 26*a(n-7) + 10*a(n-8) + a(n-9) * with
 *            a(1)=4, a(2)=10, a(3)=29, a(4)=97, a(5)=365, * a(6)=1511,
 *            a(7)=6726, a(8)=31544, a(9)=153328 * * The sequence is generated
 *            by dynamic memoisation: terms are cached * in a static a...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for previously computed values.  The array is 1‑based;
 * cache[0] is unused. */
static seq_t  *cache      = NULL;   /* pointer to the array */
static size_t  cache_cap  = 0;      /* capacity of the array in elements */
static idx_t   cache_len  = 0;      /* highest index that has been computed */

/* Initialise the first nine terms (once).  A small static array of
 * constants is used; the compiler performs the necessary truncation
 * for small seq_t types. */
static const seq_t init[9] = {
    4, 10, 29, 97, 365, 1511, 6726, 31544, 153328
};

/* Ensure that the cache can hold indices up to n (inclusive). */
static void
ensure_cache_size(idx_t n)
{
    /* No work needed if buffer already large enough. */
    if (cache_len >= n)
        return;

    /* Allocate initial buffer if necessary. */
    if (!cache) {
        cache_cap = 16;                       /* arbitrary small start */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return;                          /* out of memory: caller will get 0 */
        /* Zero the first two elements (index 0 unused, index 1 may be set later). */
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = 0;
    }

    /* Grow capacity if required. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;                    /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                               /* realloc failed */
            return;
        /* Initialise newly added elements to 0. */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Fill in the first nine initial terms if not already present. */
    for (idx_t i = 1; i <= 9 && i <= n; ++i) {
        if (i > cache_len)        /* term not yet computed */
            cache[i] = init[i - 1];
    }
    if (cache_len < 9)
        cache_len = 9;            /* we now have the first nine terms ready */

    /* Compute any remaining terms up to n using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t pos = (12 * cache[i - 1]) +
                    (65 * cache[i - 3]) +
                    (18 * cache[i - 4]) +
                    (26 * cache[i - 7]) +
                    (10 * cache[i - 8]) +
                    cache[i - 9];
        seq_t neg = (48 * cache[i - 2]) +
                    (74 * cache[i - 5]) +
                    ( 9 * cache[i - 6]);
        cache[i] = pos - neg;           /* safe: result is non‑negative */
    }
    cache_len = n;                       /* update highest computed index */
}

/* Public sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)            /* defined behaviour for 0 (undefined in OEIS) */
        return 0;

    ensure_cache_size(n);    /* make sure the requested term is available */
    return cache[n];
}
