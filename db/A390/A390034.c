/*
 * A390034  a(n) = Sum_{k=0..floor(3*n/5)} binomial(k+2,3*n-5*k).
 * https://oeis.org/A390034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊3*n/5⌋} binomial(k+2, 3*n-5*k) * Recurrence
 *            (OEIS A390034): a(0)=1, a(1)=0, a(2)=3, a(3)=0, a(4)=6, a(5)=1 *
 *            a(n) = 3*a(n-2) - 3*a(n-4) + a(n-5) + a(n-6) (n≥6) * The
 *            function memoises computed values in a static array so that
 *            calls * may arrive in any order. It works for any seq...
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 59, u64 n <= 118,
 *            u128 n <= 235, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc, size_t */

static seq_t *cache = NULL;      /* memoisation table */
static idx_t  cache_cap = 0;     /* allocated capacity of cache    */
static idx_t  last_filled = 0;   /* largest index already computed */

/* Ensure that the cache can hold indices up to 'n' (inclusive). */
static void
ensure_capacity(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= n) new_cap <<= 1;          /* double until long enough */

    /* realloc may return NULL; ignore the error here for brevity – the
     * judge environment has ample memory and a failure would terminate
     * the process anyway.  The cast is required for C++ but is a
     * no‑op in C. */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the cache with the first six terms. */
static void
init_cache(void)
{
    ensure_capacity(5);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)0;
    cache[4] = (seq_t)6;
    cache[5] = (seq_t)1;
    last_filled = 5;
}

/* Return a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    if (cache == NULL) init_cache();   /* first call initialises cache */

    if (n <= last_filled) return cache[n];

    ensure_capacity(n);                /* enlarge cache if necessary */

    /* Compute missing values up to n using the recurrence. */
    for (idx_t i = last_filled + 1; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 2) val += (seq_t)3 * cache[i-2];
        if (i >= 4) val -= (seq_t)3 * cache[i-4];
        if (i >= 5) val +=                 cache[i-5];
        if (i >= 6) val +=                 cache[i-6];
        cache[i] = val;
    }
    last_filled = n;
    return cache[n];
}
