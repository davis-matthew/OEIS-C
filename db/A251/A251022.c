/*
 * A251022  Number of (n+1)X(2+1) 0..3 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements greater than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) -25*a(n-2) +35*a(n-3) -7*a(n-4) -49*a(n-5)
 *            +77*a(n-6) -55*a(n-7) +20*a(n-8) -3*a(n-9) for n>13 Initial
 *            values: a(1)=526, a(2)=2546, a(3)=8207, a(4)=23404, a(5)=60044,
 *            a(6)=144737, a(7)=334167, a(8)=755336, a(9)=1690491,
 *            a(10)=3781186, a(11)=8485355, a(12)=19144607, a(13)=43420578...
 * Range:     u8 none, u16 n <= 5, u32 n <= 18, u64 n <= 45, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

static const seq_t init_vals[13] = {
    526,   2546,   8207,   23404,   60044,
    144737,334167,755336,1690491,3781186,
    8485355,19144607,43420578
};

static seq_t *cache = NULL;      /* 1‑based array: cache[1] = a(1) */
static size_t cache_cap = 0;     /* allocated size (including 0 index)   */
static size_t cache_len = 0;     /* largest index that has a value stored */

/* Initialise cache with the first 13 terms. */
static void init_cache(void)
{
    cache_cap = 14;                          /* indices 0 … 13  */
    cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
    for (size_t i = 1; i <= 13; ++i) cache[i] = init_vals[i - 1];
    cache_len = 13;
}

/* Ensure the cache is large enough to contain index n. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) init_cache();

    if ((idx_t)n >= (idx_t)cache_cap) {
        size_t new_cap = cache_cap;
        while ((idx_t)n >= (idx_t)new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed; terminate */ abort(); }
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }
}

/* Compute the sequence up to index n (n > cache_len) using the recurrence. */
static void compute_until(idx_t n)
{
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];
        seq_t a7 = cache[i - 7];
        seq_t a8 = cache[i - 8];
        seq_t a9 = cache[i - 9];

        seq_t pos = 8 * a1 + 35 * a3 + 77 * a6 + 20 * a8;
        seq_t neg = 25 * a2 + 7 * a4 + 49 * a5 + 55 * a7 + 3 * a9;
        cache[i]  = pos - neg;       /* pos >= neg for all n */
    }
    cache_len = n;
}

/* External function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* sequence is not defined for n<1 */

    ensure_cache(n);
    if ((idx_t)cache_len < n) compute_until(n);
    return cache[n];
}
