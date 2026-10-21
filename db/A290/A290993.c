/*
 * A290993  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - S^6.
 * https://oeis.org/A290993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0, for n < 5; a(5) = 1; a(n) = 6*a(n-1) - 15*a(n-2) +
 *            20*a(n-3) - 15*a(n-4) + 6*a(n-5), for n > 5. Initial values:
 *            a(0)=a(1)=a(2)=a(3)=a(4)=0, a(5)=1. The implementation uses
 *            memoisation with an automatically grown static array
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoisation array */
static idx_t capacity = 0;    /* allocated size of cache array */
static idx_t last_computed = 5; /* highest index that has been computed */

/* Ensure the cache has space for at least (n+1) elements */
static void ensure_capacity(idx_t n)
{
    if (n < capacity)
        return;

    idx_t new_cap = capacity ? (capacity << 1) : 16;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return; /* allocation failed: in this context we just return */

    for (idx_t i = 0; i < capacity; ++i)
        new_cache[i] = cache[i];

    free(cache);
    cache = new_cache;
    capacity = new_cap;
}

/* Initialise the first 6 terms of the sequence */
static void init_sequence(void)
{
    if (cache)
        return; /* already initialised */

    ensure_capacity(5);
    for (idx_t i = 0; i < 5; ++i) cache[i] = 0;
    cache[5] = 1;
    last_computed = 5;
}

seq_t a(idx_t n)
{
    /* Handle trivial cases directly */
    if (n < 5)
        return 0;
    if (n == 5)
        return 1;

    init_sequence();          /* Ensure cache exists and first values are set */

    /* Grow the cache if necessary */
    ensure_capacity(n);

    /* Compute terms up to n incrementally */
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        /* Use two sums to avoid unsigned underflow: positive part - negative part */
        seq_t pos = ((seq_t)6 * cache[i - 1]) + ((seq_t)20 * cache[i - 3]) + ((seq_t)6 * cache[i - 5]);
        seq_t neg = ((seq_t)15 * cache[i - 2]) + ((seq_t)15 * cache[i - 4]);

        cache[i] = pos - neg;
    }

    last_computed = n;
    return cache[n];
}
