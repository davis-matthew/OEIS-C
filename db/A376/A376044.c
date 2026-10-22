/*
 * A376044  a(0)=1; thereafter, a(n) = denominator of Sum_{i=1..n}
 *          A376043(i)/A376043(i+1).
 * https://oeis.org/A376044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator of Sum_{i=1..n} A376043(i)/A376043(i+1) For
 *            n>0: a(n) = A376043(n)*a(n-1)^2 + a(n-1) a(0) = 1 The function
 *            uses memoised computation so that calls in any order are
 *            efficient and correct for every n ≥ 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Forward declarations for memoisation */
static seq_t *cache = NULL;        /* array of a(i) values */
static idx_t  cache_cap = 0;       /* current capacity of cache */
static idx_t  max_computed = 0;    /* largest index computed */

/* Ensure that the cache can hold indices up to n */
static void
ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate space for at least 2 entries and for n+1 in case n > 1 */
        idx_t cap = 2;
        if (n + 1 > cap) cap = n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);   /* fatal: out of memory */
        cache_cap = cap;
        cache[0] = (seq_t)1;
        if (cache_cap > 1) cache[1] = (seq_t)2;
        max_computed = 1;
    } else if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (n + 1 > new_cap) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
        /* max_computed remains unchanged */
    }
}

/* Main function computing a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Make sure cache is large enough */
    ensure_cache(n);

    /* If already computed, return it immediately */
    if (n <= max_computed) return cache[n];

    /* Compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t a_prev1 = cache[i - 1];
        seq_t a_prev2 = cache[i - 2];
        seq_t square  = a_prev1 * a_prev1;
        seq_t div     = square / a_prev2;          /* exact division */
        seq_t term    = div + (seq_t)1;
        cache[i] = a_prev1 * term;
    }
    max_computed = n;
    return cache[n];
}
