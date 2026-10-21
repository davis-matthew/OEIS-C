/*
 * A266543  Number of n X 4 binary arrays with rows and columns
 *          lexicographically nondecreasing and row and column sums
 *          nonincreasing.
 * https://oeis.org/A266543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3) - a(n-4) - a(n-5) - a(n-6) +
 *            2*a(n-8) + 2*a(n-9) - a(n-11) - a(n-12) - a(n-13) - a(n-14) +
 *            2*a(n-15) + a(n-16) - a(n-17) Initial values: a(1)=2, a(2)=4,
 *            a(3)=6, a(4)=12, a(5)=16, a(6)=27, a(7)=36, a(8)=57, a(9)=76,
 *            a(10)=114, a(11)=149, a(12)=213, a(13)=276, a(14...
 * Range:     u8 n <= 12, u16 n <= 44, u32 n <= 179, u64 n <= 179,
 *            u128 n <= 179, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Dynamic cache for computed terms */
static seq_t *cache = NULL;     /* array a(1)..a(cache_size) */
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap   = 0;  /* allocated capacity */

/* Forward declaration of helper for resizing */
static void grow_cache(size_t needed);

/* Initialise cache with known first 17 terms */
static void init_cache(void)
{
    if (cache != NULL)
        return;                /* already initialised */

    cache_cap = 32;             /* initial capacity */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();

    /* 17 known terms, 1‑based */
    const seq_t first17[17] = {
        (seq_t)2,   (seq_t)4,   (seq_t)6,   (seq_t)12,  (seq_t)16,
        (seq_t)27,  (seq_t)36,  (seq_t)57,  (seq_t)76,  (seq_t)114,
        (seq_t)149, (seq_t)213, (seq_t)276, (seq_t)379, (seq_t)485,
        (seq_t)645, (seq_t)811
    };
    for (size_t i = 0; i < 17; ++i)
        cache[i] = first17[i];

    cache_size = 17;
}

/* Resize cache if required */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for offset 0 */

    init_cache();

    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1];

    /* Compute terms up to n */
    grow_cache((size_t)n);
    for (size_t k = cache_size + 1; k <= (size_t)n; ++k)
    {
        /* Accumulate positive and negative parts separately
           to avoid underflow in unsigned types.  The recurrence
           guarantees that the positive part is always >=
           the negative part. */
        seq_t pos = 0, neg = 0;

        /* Positive coefficients */
        /* + a(k-1) */
        pos += cache[k - 2];
        /* + 2*a(k-2) */
        pos += cache[k - 3];
        pos += cache[k - 3];
        /* + 2*a(k-8) */
        pos += cache[k - 9];
        pos += cache[k - 9];
        /* + 2*a(k-9) */
        pos += cache[k - 10];
        pos += cache[k - 10];
        /* + 2*a(k-15) */
        pos += cache[k - 16];
        pos += cache[k - 16];
        /* + a(k-16) */
        pos += cache[k - 17];

        /* Negative coefficients (-1 for each) */
        neg += cache[k - 4];   /* a(k-3) */
        neg += cache[k - 5];   /* a(k-4) */
        neg += cache[k - 6];   /* a(k-5) */
        neg += cache[k - 7];   /* a(k-6) */
        neg += cache[k - 12];  /* a(k-11) */
        neg += cache[k - 13];  /* a(k-12) */
        neg += cache[k - 14];  /* a(k-13) */
        neg += cache[k - 15];  /* a(k-14) */
        neg += cache[k - 18];  /* a(k-17) */

        cache[k - 1] = pos - neg;   /* guaranteed non‑negative */
    }

    cache_size = (size_t)n;
    return cache[(size_t)n - 1];
}
