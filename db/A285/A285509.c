/*
 * A285509  a(1) = 1; a(2) = a(3) = a(4) = 2; a(n) = a(a(n-1)-1) + a(n-a(n-3))
 *          for n > 4.
 * https://oeis.org/A285509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 332, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(2)=a(3)=a(4)=2;
   a(n)=a(a(n-1)-1)+a(n-a(n-3))  for n>4 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed terms, 1‑based */
static size_t cache_cap = 0;  /* allocated capacity (index 0 unused) */
static idx_t  filled = 0;     /* largest computed index */

/* ensure cache has room for indices 0..n */
static void grow_cache_to(idx_t n)
{
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* compute a(n) for all n ≥ 1, memoised */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of range – not used */

    /* initialise the first four terms on first call */
    if (filled == 0) {
        grow_cache_to(4);
        cache[1] = (seq_t)1;
        cache[2] = cache[3] = cache[4] = (seq_t)2;
        filled = 4;
    }

    if (n <= filled) return cache[n];

    grow_cache_to(n);

    for (idx_t i = filled + 1; i <= n; ++i) {
        /* term1 = a(a(i-1)-1) */
        idx_t a_i_1 = (idx_t)cache[i - 1];          /* a(i-1) ≥ 1 */
        idx_t idx1  = a_i_1 - 1;                   /* ≥0 */
        seq_t term1 = cache[(size_t)idx1];

        /* term2 = a(i - a(i-3)) */
        idx_t a_i_3 = (idx_t)cache[i - 3];          /* ≥1 for i≥5 */
        idx_t idx2  = i - a_i_3;                   /* ≥1 */
        seq_t term2 = cache[(size_t)idx2];

        cache[i] = term1 + term2;
    }

    filled = n;
    return cache[n];
}
