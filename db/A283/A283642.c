/*
 * A283642  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 678", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n+2) + (-1)^n)/3 Recurrence: a(0)=1, a(1)=3,
 *            a(n)=a(n-1)+2*a(n-2) for n>1 (Precomputes values with memoised
 *            cache. Works for any integer seq_t type that supports the usual
 *            arithmetic operators.)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised values */
static idx_t  cache_len = 0;  /* number of elements stored */

/* Ensure that cache[0..n] are computed. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_len)                 /* already ready */
        return;

    /* Grow the cache to hold indices up to n. */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc((void *)cache,
                                         new_len * sizeof(seq_t));
    if (!new_cache)
        abort();                       /* OOM – terminate */
    cache = new_cache;

    /* Initialise base cases if we have just created the array. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;           /* a(0) */
        cache_len = 1;
    }
    if (n >= 1 && cache_len < 2) {
        cache[1] = (seq_t)3;           /* a(1) */
        cache_len = 2;
    }

    /* Compute any remaining values up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] + (seq_t)2 * cache[i-2];
    }

    cache_len = new_len;
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
