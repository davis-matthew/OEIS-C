/*
 * A325153  A column of triangle A322220; a(n) = A322220(n,1) for n >= 1.
 * https://oeis.org/A325153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9^n - 9)/24 + n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = 9*a(n-1) - 8*n + 12  (n >= 2) */

#include <stdlib.h>      /* for realloc, exit, EXIT_FAILURE */

/* static cache for memoization */
static seq_t *cache = NULL;   /* 1-indexed array: cache[1] = a(1) */
static size_t cached = 0;     /* highest index currently computed */
static size_t cap = 0;        /* allocated capacity (number of elements-1) */

/* ensure that terms up to n are computed and stored in cache */
static void ensure_n(idx_t n)
{
    if (n <= cached) return;          /* already computed */

    /* determine new capacity (at least n) */
    size_t new_cap = n;
    if (new_cap < 2 * cap) new_cap = 2 * cap;
    if (new_cap < 1) new_cap = 1;      /* never zero */

    /* reallocate cache to hold indices 0..new_cap */
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – abort */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cap = new_cap;

    /* initialise from scratch if first run */
    if (cached == 0) {
        cache[1] = (seq_t)1;          /* a(1) */
        cached = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t term = prev * (seq_t)9;          /* 9 * a(i-1) */
        term -= (seq_t)8 * (seq_t)i;           /* - 8*i */
        term += (seq_t)12;                     /* + 12 */
        cache[i] = term;
    }
    cached = n;
}

/* exportable function required by the framework */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, return 0 */
    ensure_n(n);
    return cache[n];
}
