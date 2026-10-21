/*
 * A258470  Number of partitions of n of order 10.
 * https://oeis.org/A258470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence derived from the generating
 *            function G(x) = (-x^17 + x^10 + x^7) /
 *            ((1-x)(1-x^2)(1-x^5)(1-x^10)): a(n) = a(n-1) + a(n-2) - a(n-3) +
 *            a(n-5) - a(n-6) - a(n-7) + a(n-8) + a(n-10) - a(n-11) - a(n-12)
 *            + a(n-13) - a(n-15) + a(n-16) + a(n-17) - a(n-18) for n ≥ 18
 *            Ini...
 * Range:     u8 n <= 29, u16 n <= 206, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Initial values a(0) … a(17) */
static const seq_t init[18] = {
    0ULL, /* a(0) */
    0ULL, /* a(1) */
    0ULL, /* a(2) */
    0ULL, /* a(3) */
    0ULL, /* a(4) */
    0ULL, /* a(5) */
    0ULL, /* a(6) */
    1ULL, /* a(7) */
    1ULL, /* a(8) */
    2ULL, /* a(9) */
    3ULL, /* a(10) */
    4ULL, /* a(11) */
    6ULL, /* a(12) */
    7ULL, /* a(13) */
    9ULL, /* a(14) */
    11ULL,/* a(15) */
    13ULL,/* a(16) */
    16ULL /* a(17) */
};

/* Coefficients corresponding to the recurrence:
   coeff[k] == +1 : add a(n-k)
   coeff[k] == -1 : subtract a(n-k)
   coeff[k] ==  0 : term does not appear
   Indices are 1‑based; coeff[0] is unused. */
static const signed char coeff[19] = {
    0,   /* unused */
    1,   /* +a(n-1) */
    1,   /* +a(n-2) */
   -1,   /* -a(n-3) */
    0,   /* no a(n-4) */
    1,   /* +a(n-5) */
   -1,   /* -a(n-6) */
   -1,   /* -a(n-7) */
    1,   /* +a(n-8) */
    0,   /* no a(n-9) */
    1,   /* +a(n-10) */
   -1,   /* -a(n-11) */
   -1,   /* -a(n-12) */
    1,   /* +a(n-13) */
    0,   /* no a(n-14) */
   -1,   /* -a(n-15) */
    1,   /* +a(n-16) */
    1,   /* +a(n-17) */
   -1    /* -a(n-18) */
};

seq_t a(idx_t n)
{
    /* Return trivial initial values directly */
    if (n < 18)
        return init[n];

    /* Static memoisation cache */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cache_size = 0;  /* number of entries computed */

    /* Ensure capacity for index n */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (required > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base values on first use */
    if (cache_size < 18) {
        for (idx_t i = cache_size; i < 18; ++i)
            cache[i] = init[i];
        cache_size = 18;
    }

    /* Compute missing terms up to 'n' */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 1; k <= 18; ++k) {
            seq_t term = cache[i - k];
            if (coeff[k] == 1)
                pos += term;
            else if (coeff[k] == -1)
                neg += term;
            /* coeff[k] == 0 → skip */
        }
        cache[i] = pos;
        if (pos >= neg)
            cache[i] -= neg;
        else
            cache[i] = 0;  /* should not happen for this sequence */
    }

    cache_size = n + 1;
    return cache[n];
}
