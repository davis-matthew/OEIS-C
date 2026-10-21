/*
 * A276534  a(n) = a(n-1) * a(n-4) * (a(n-2) * a(n-3) + 1) / a(n-5), with a(0)
 *          = a(1) = a(2) = a(3) = a(4) = 1.
 * https://oeis.org/A276534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * a(n-4) * (a(n-2) * a(n-3) + 1) / a(n-5), a(0) =
 *            a(1) = a(2) = a(3) = a(4) = 1
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 10, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function requires the C11/ C++17 header <stdlib.h> for malloc/realloc. */
#include <stdlib.h>

/* Helper: multiply two seq_t values using a 128‑bit intermediate when compiling C
   with __uint128_t available.  In C++ builds (where seq_t is an arbitrary‑precision
   class) we fall back to ordinary multiplication. */
static inline seq_t mul128(seq_t x, seq_t y)
{
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
    return (seq_t)(((__uint128_t)x) * ((__uint128_t)y));
#else
    return x * y;
#endif
}

/* memoisation table */
static seq_t *cache = NULL;     /* array of computed a(n) values */
static idx_t  cache_size = 0;   /* number of valid entries in cache */

/* sequence a(n) */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n <= 4) return (seq_t)1;

    /* Initialise cache for the first computation */
    if (!cache) {
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        for (idx_t i = 0; i < 5; ++i) cache[i] = (seq_t)1;
    }

    /* Return already computed value */
    if (n < cache_size) return cache[n];

    /* Grow cache to hold values up to n */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                /* allocation failed */
    cache = tmp;
    for (idx_t i = cache_size; i <= n; ++i) cache[i] = (seq_t)0;
    cache_size = n + 1;

    /* Compute missing values */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];

        /* (a2 * a3) + 1 */
        seq_t prod12 = mul128(a2, a3);
        seq_t prod12_plus = prod12 + (seq_t)1;

        /* a1 * a4 */
        seq_t prod14 = mul128(a1, a4);

        /* numerator: a1 * a4 * (a2 * a3 + 1) */
        seq_t num = mul128(prod14, prod12_plus);

        /* division */
        cache[i] = num / a5;
    }

    return cache[n];
}
