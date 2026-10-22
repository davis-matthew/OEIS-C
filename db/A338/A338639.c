/*
 * A338639  a(0) = 1; for n > 0, a(n) = -Sum_{d|n, d < n} a(d - 1).
 * https://oeis.org/A338639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A338639
   Recurrence:
   a(0) = 1
   for n > 0, a(n) = -∑_{d|n, d < n} a(d-1)
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr)
{
    /* enlarge the cache so that it contains at least indices 0..n */
    size_t needed = (size_t)n + 1;
    size_t cap = *cap_ptr;
    if (cap >= needed)
        return;
    size_t new_cap = cap ? cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(*cache_ptr, new_cap * sizeof(seq_t));
    if (!new_arr)
        abort();          /* memory allocation failed */
    *cache_ptr = new_arr;
    *cap_ptr = new_cap;
}

/* static cache for computed values */
static seq_t *cache = NULL;
static size_t cap    = 0;   /* capacity of the cache array */
static idx_t  last_computed = 0;   /* highest n for which cache[n] is valid */

/* Main entry point */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) == 1 */

    ensure_cache(n, &cache, &cap);

    /* initialise a(0) if this is the first call having n>0 */
    if (cache[0] == 0)
        cache[0] = (seq_t)1;

    /* if we've already computed up to n, just return the value */
    if (n <= last_computed)
        return cache[(size_t)n];

    /* compute terms from last_computed+1 up to n */
    for (idx_t m = last_computed + 1; m <= n; ++m) {
        seq_t sum = 0;
        /* enumerate divisors of m */
        for (idx_t i = 1; i <= m / i; ++i) {
            if (m % i == 0) {
                idx_t d1 = i;
                idx_t d2 = m / i;
                if (d1 < m) {
                    sum += cache[(size_t)(d1 - 1)];
                }
                if (d2 != d1 && d2 < m) {
                    sum += cache[(size_t)(d2 - 1)];
                }
            }
        }
        cache[(size_t)m] = -sum;
    }

    last_computed = n;
    return cache[(size_t)n];
}
