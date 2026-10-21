/*
 * A249962  Number of length 2+5 0..n arrays with no six consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A249962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) Initial terms: a(1)=15,
 *            a(2)=505, a(3)=5300, a(4)=31180, a(5)=129095, a(6)=422065,
 *            a(7)=1164800, a(8)=2830080
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* stored values a(1)…a(max)   */
static size_t cache_cap = 0;       /* allocated capacity (elements) */
static idx_t cache_max = 0;        /* highest index computed so far */

/* Compute the recurrence for index i (i >= 9). Assumes c is filled up to i-1. */
static seq_t rec(const seq_t *c, idx_t i)
{
    /* All arithmetic is performed in seq_t so it works
       for both built‑in integer types and arbitrary‑precision
       big integer classes. */
    return 8 * c[(size_t)(i - 1)] - 28 * c[(size_t)(i - 2)]
         + 56 * c[(size_t)(i - 3)] - 70 * c[(size_t)(i - 4)]
         + 56 * c[(size_t)(i - 5)] - 28 * c[(size_t)(i - 6)]
         + 8  * c[(size_t)(i - 7)] - c[(size_t)(i - 8)];
}

/* External function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n=0, return 0 to avoid UB */
        return 0;

    /* Base values for indices 1 through 8.  Cast literals so they work
       with both integral types and arbitrary‑precision classes. */
    static const seq_t base[9] = {
        (seq_t)0ull,          /* unused 0‑th element */
        (seq_t)15ull,         /* a(1) */
        (seq_t)505ull,        /* a(2) */
        (seq_t)5300ull,       /* a(3) */
        (seq_t)31180ull,      /* a(4) */
        (seq_t)129095ull,     /* a(5) */
        (seq_t)422065ull,     /* a(6) */
        (seq_t)1164800ull,    /* a(7) */
        (seq_t)2830080ull     /* a(8) */
    };

    /* Lazy cache allocation on first call. */
    if (cache == NULL) {
        cache_cap = 16; /* small initial capacity; grows as needed */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return 0;   /* allocation failed; return 0 as a fallback */
        /* Initialize cache to zero and set base values. */
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = 0;
        for (idx_t i = 1; i <= 8; ++i)
            cache[(size_t)i] = base[(size_t)i];
        cache_max = 8;
    }

    /* Grow cache if requested index is beyond current capacity. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0;   /* allocation failed */
        /* Initialize the newly allocated part to zero. */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to the requested index. */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i <= 8)
                cache[(size_t)i] = base[(size_t)i];
            else
                cache[(size_t)i] = rec(cache, i);
        }
        cache_max = n;
    }

    return cache[(size_t)n];
}
