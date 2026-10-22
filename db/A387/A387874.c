/*
 * A387874  a(n) = Sum_{k=0..n} binomial(2*n+1,6*k).
 * https://oeis.org/A387874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A387874
 * Recurrence: a(n) = 6*a(n-1) - 15*a(n-2) + 22*a(n-3) + 15*a(n-4) + 36*a(n-5)
 * Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=8, a(4)=85
 */

#include <stdlib.h>

/* memoization table */
static seq_t *cache = NULL;          /* cache[n] holds a(n) */
static size_t cache_len = 0;         /* length of allocated cache */
static idx_t max_cached = 0;         /* highest n whose value is already computed */

/* Ensure that the cache can hold indices up to 'need' (inclusive). */
static void ensure_cache(idx_t need)
{
    size_t ns = (size_t)need;
    if (ns < cache_len) return;           /* already large enough */

    size_t new_len = cache_len ? cache_len : 5;
    while (new_len <= ns) new_len <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) abort();              /* out of memory */
    cache = new_cache;
    cache_len = new_len;
}

/* Initialise the base terms. */
static void init_base(void)
{
    if (cache) return;                    /* already initialised */

    ensure_cache(4);                      /* need indices 0..4 */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)8;
    cache[4] = (seq_t)85;
    max_cached = 4;
}

/* Compute a(n).  Works for any build that defines seq_t and idx_t. */
seq_t a(idx_t n)
{
    init_base();

    if (n <= max_cached) return cache[n];

    ensure_cache(n);

    for (idx_t i = max_cached + 1; i <= n; ++i)
    {
        seq_t sum = cache[i-1] * (seq_t)6;
        sum -= cache[i-2] * (seq_t)15;
        sum += cache[i-3] * (seq_t)22;
        sum += cache[i-4] * (seq_t)15;
        sum += cache[i-5] * (seq_t)36;
        cache[i] = sum;
    }

    max_cached = n;
    return cache[n];
}
