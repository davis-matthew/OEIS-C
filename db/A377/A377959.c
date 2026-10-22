/*
 * A377959  Expansion of e.g.f. exp(x - x^2)/(1 - x)^2.
 * https://oeis.org/A377959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1, a(1) = 3, a(2) = 9
   a(n) = (n+2)*a(n-1) - 3*(n-1)*a(n-2) + 2*(n-1)*(n-2)*a(n-3)   (n > 2) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* current capacity of cache */

    /* If the value is already cached, return it immediately */
    if (n < cap)
        return cache[n];

    /* enlarge cache to hold indices up to n */
    idx_t old_cap = cap;
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return 0;          /* allocation failed – unlikely in tests */
    cache = tmp;
    cap   = new_cap;

    /* initialise base values if not already present */
    if (cap > 0)  cache[0] = (seq_t)1;
    if (cap > 1)  cache[1] = (seq_t)3;
    if (cap > 2)  cache[2] = (seq_t)9;

    /* compute missing terms only */
    idx_t start = (old_cap > 3 ? old_cap : 3);
    for (idx_t i = start; i <= n; ++i)
    {
        seq_t term1 = (seq_t)(i + 2) * cache[i - 1];
        seq_t term2 = (seq_t)3 * (seq_t)(i - 1) * cache[i - 2];
        seq_t term3 = (seq_t)2 * (seq_t)(i - 1) * (seq_t)(i - 2) * cache[i - 3];
        cache[i] = term1 - term2 + term3;
    }

    return cache[n];
}
