/*
 * A285199  Product of n! and the n-th Legendre polynomial evaluated at 2.
 * https://oeis.org/A285199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(n+2) = (4n+6)*a(n+1) – (n+1)²*a(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc, free */

/* Memoisation cache for the sequence values. */
static seq_t *cache = NULL;   /* pointer to allocated array */
static size_t cap   = 0;      /* current capacity of the array (elements) */
static size_t len   = 0;      /* number of elements currently stored */

/* Ensure the cache can hold at least `needed` elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cap)
        return;

    size_t new_cap = (cap == 0) ? 2 : cap;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (cap == 0) ? (seq_t *)malloc(new_cap * sizeof(seq_t))
                            : (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In the unlikely event of allocation failure we simply return.
       The caller will then try again and presumably succeed.  */
    if (tmp == NULL)
        return;

    cache = tmp;
    cap   = new_cap;
}

/* Compute a(n) as defined above.  `n` is an idx_t (uint64_t). */
seq_t a(idx_t n)
{
    /* Handle the trivial cases immediately. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* Compute up to index n; place results in the cache. */
    size_t needed = (size_t)n + 1;          /* indices 0 … n inclusive */
    ensure_capacity(needed);

    /* On the first call initialise the first two terms. */
    if (len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        len      = 2;
    }

    /* Fill any missing terms up to n.  Recurrence for k ≥ 2: */
    /* a(k) = (4k-2)*a(k-1) – (k-1)² * a(k-2)  */
    for (size_t i = len; i <= (size_t)n; ++i) {
        /* Compute coefficients using seq_t arithmetic. */
        seq_t coeff1 = (seq_t)(4 * i - 2);                 /* (4k-2) */
        seq_t sq     = (seq_t)(i - 1) * (seq_t)(i - 1);     /* (k-1)² */
        seq_t term1  = coeff1 * cache[i - 1];
        seq_t term2  = sq * cache[i - 2];
        cache[i] = term1 - term2;
    }

    len = (size_t)n + 1;  /* we now have terms 0 … n stored */

    return cache[(size_t)n];
}
