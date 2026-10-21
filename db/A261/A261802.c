/*
 * A261802  Number of 10-compositions of n: matrices with 10 rows of
 *          nonnegative integers with positive column sums and total element
 *          sum n.
 * https://oeis.org/A261802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k>=0} (1/2)^(k+1) * binomial(n-1+10*k, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence satisfies the 10‑term recurrence
   a(n) = 20*a(n-1) - 90*a(n-2) + 240*a(n-3) - 420*a(n-4) + 504*a(n-5)
          - 420*a(n-6) + 240*a(n-7) - 90*a(n-8) + 20*a(n-9) - 2*a(n-10)
   for n >= 10, with the first 11 terms given below.  The implementation
   keeps a cache of computed values, allocating more space only when
   necessary.  All arithmetic is performed in seq_t with no overflow
   checks (as required).  The routine works for any seq_t, even
   arbitrarily large, because it relies only on the recurrence. */

#include <stdlib.h>
#include <stddef.h>

/* initial 11 terms */
static const seq_t init_11[11] = {
    (seq_t)1U,
    (seq_t)10U,
    (seq_t)155U,
    (seq_t)2320U,
    (seq_t)34640U,
    (seq_t)517252U,
    (seq_t)7723970U,
    (seq_t)115339960U,
    (seq_t)1722340115ULL,
    (seq_t)25719233330ULL,
    (seq_t)384058268507ULL
};

/* dynamic cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* current allocation size (in terms) */
static idx_t cache_len = 0;    /* number of terms currently stored */

/* Ensure the cache has capacity for at least `needed` terms. */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (needed <= *cap)
        return;
    size_t newcap = *cap ? *cap : 8;
    while (newcap < needed)
        newcap <<= 1;                     /* double until big enough */
    seq_t *newp = (seq_t *)realloc(*ptr, newcap * sizeof(seq_t));
    if (newp == NULL)                       /* allocation failed */
        exit(1);
    *ptr = newp;
    *cap = newcap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* return precomputed first 11 terms directly */
    if (n < 11)
        return init_11[n];

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            exit(1);
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = init_11[i];
        cache_len = 11;
    }

    /* grow cache if needed */
    if ((size_t)n >= cache_len) {
        ensure_capacity((size_t)n + 1, &cache_cap, &cache);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = cache[i-1] * (seq_t)20;
            sum -= cache[i-2] * (seq_t)90;
            sum += cache[i-3] * (seq_t)240;
            sum -= cache[i-4] * (seq_t)420;
            sum += cache[i-5] * (seq_t)504;
            sum -= cache[i-6] * (seq_t)420;
            sum += cache[i-7] * (seq_t)240;
            sum -= cache[i-8] * (seq_t)90;
            sum += cache[i-9] * (seq_t)20;
            sum -= cache[i-10] * (seq_t)2;
            cache[i] = sum;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
