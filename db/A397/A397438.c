/*
 * A397438  Growth function of the reflection group of a six-dimensional
 *          orthoscheme with distinct edge lengths.
 * https://oeis.org/A397438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-3) for n >= 5 with initial values
 *            a(0)=1, a(1)=7, a(2)=27, a(3)=82, a(4)=226
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached terms */
static size_t cache_cap = 0;     /* capacity of cache array */
static idx_t cache_sz = 0;       /* number of terms already computed */

/* Return the nth term of A397438.  The sequence is memoized in the
   static array `cache`.  The function can be called in any order
   and will compute missing terms on demand. */
seq_t a(idx_t n)
{
    /* If we already have the value, just return it. */
    if (n < cache_sz)
        return cache[n];

    /* Extend the cache to hold index n. */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) { abort(); }
        cache     = tmp;
        cache_cap = needed;
    }

    /* Compute all new terms up to n. */
    for (idx_t i = cache_sz; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)7;
        } else if (i == 2) {
            cache[i] = (seq_t)27;
        } else if (i == 3) {
            cache[i] = (seq_t)82;
        } else if (i == 4) {
            cache[i] = (seq_t)226;
        } else {
            cache[i] = cache[i - 1] * (seq_t)3 - cache[i - 3] * (seq_t)3;
        }
    }

    cache_sz = n + 1;
    return cache[n];
}
