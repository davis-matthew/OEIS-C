/*
 * A287422  a(1) = a(2) = 1; a(n) = n - a(a(n-1)) - a(n-a(n-1)) for n > 2.
 * https://oeis.org/A287422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(2) = 1; a(n) = n - a(a(n-1)) - a(n-a(n-1)) for n>2
 * Range:     u8 n <= 506, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (n == 0) return;
    if (n <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
    while (new_cap < n) new_cap <<= 1;  /* double until large enough */

    seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    if (!new_cache) exit(1);

    /* copy existing entries */
    for (idx_t i = 0; i <= cache_cap; ++i)
        new_cache[i] = seq_cache ? seq_cache[i] : 0;

    /* initialize new entries to 0 */
    for (idx_t i = cache_cap + 1; i <= new_cap; ++i)
        new_cache[i] = 0;

    free(seq_cache);
    seq_cache = new_cache;
    cache_cap  = new_cap;
}

/* return a(n) for n >= 1, memoised */
seq_t a(idx_t n)
{
    if (n == 1 || n == 2) return (seq_t)1;

    ensure_cache(n);

    /* initialise the first two terms if not yet stored */
    if (seq_cache[1] == 0) seq_cache[1] = (seq_t)1;
    if (seq_cache[2] == 0) seq_cache[2] = (seq_t)1;

    if (seq_cache[n]) return seq_cache[n];

    seq_t prev_term = a(n - 1);

    /* a(a(n-1)) */
    idx_t idx1 = (idx_t)prev_term;
    seq_t val1 = a(idx1);

    /* a(n - a(n-1)) */
    idx_t idx2 = n - idx1;
    seq_t val2 = a(idx2);

    seq_t result = (seq_t)n - val1 - val2;

    seq_cache[n] = result;
    return result;
}
