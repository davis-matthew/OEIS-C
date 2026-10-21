/*
 * A295682  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 0, a(1) = 2, a(2) =
 *          0, a(3) = 1.
 * https://oeis.org/A295682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4) a(0) = 0, a(1) = 2, a(2) = 0,
 *            a(3) = 1 Memoized recursive computation with static cache
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache and the highest index that has been computed */
static seq_t *cache = NULL;
static idx_t  high   = 0;   /* indices 0 .. high are known */

/* ensure that the cache can hold indices up to 'n' (inclusive) */
static void ensure_cap(idx_t n)
{
    if (n <= high)
        return;

    idx_t new_cap = n + 1;                        /* allocate for 0 .. n */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (tmp == NULL)
        exit(EXIT_FAILURE);                      /* out of memory */
    cache = tmp;
}

/* initialize the base values 0..3 if they are not already in the cache */
static void init_base(void)
{
    if (high >= 3)
        return;                                  /* already set */

    ensure_cap(3);                               /* at least capacity 4 */
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)0;
    cache[3] = (seq_t)1;
    high     = 3;                                /* indices 0..3 known */
}

seq_t a(idx_t n)
{
    /* Direct handling of the first four terms */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;

    /* Make sure the cache is ready and the base values are set */
    init_base();

    /* If the requested value is already computed, return it */
    if (n <= high)
        return cache[n];

    /* Expand the cache to cover the requested index */
    ensure_cap(n);

    /* Compute missing terms using the recurrence */
    for (idx_t i = high + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        if (i >= 3) val += cache[i - 3];
        if (i >= 4) val += cache[i - 4];
        cache[i] = val;
    }
    high = n;        /* all indices up to n are now known */

    return cache[n];
}
