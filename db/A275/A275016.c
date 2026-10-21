/*
 * A275016  a(n) = (2^n - (-1+i)^n - (-1-i)^n)/4 - 1 where i is the imaginary
 *          unit.
 * https://oeis.org/A275016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A275016
   Recurrence: a(n) = a(n-1) + 2*a(n-2) + 2*a(n-3) - 4*a(n-4) for n>4
   Initial values: a(1)=0, a(2)=0, a(3)=0, a(4)=5
*/

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoised values, indexed by n (>=1) */
static idx_t cached_until = 0;       /* highest n computed and stored in cache */

/* Helper to obtain base value for n <= 4 without allocating cache */
static inline seq_t base_value(idx_t n)
{
    static const seq_t init[5] = {0, 0, 0, 0, 5};  /* indices 0..4 */
    return init[n];
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* undefined for n < 1, but safe */

    if (n <= 4)               /* return initial terms directly */
        return base_value(n);

    /* ensure cache large enough */
    if (cache == NULL) {
        /* allocate for indices 0..n (ignore index 0) */
        size_t cap = (size_t)(n + 1);
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (cache == NULL)
            abort();        /* allocation failure */
        /* populate base values */
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 5;
        cached_until = 4;
    } else if (n > cached_until) {
        /* extend allocation if needed */
        size_t new_cap = (size_t)(n + 1);
        if (new_cap > (size_t)cached_until + 1) {
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (new_cache == NULL)
                abort();      /* allocation failure */
            cache = new_cache;
        }
        /* no need to re‑set base values; they already exist */
    }

    /* compute missing terms until n */
    for (idx_t i = cached_until + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + (seq_t)2 * cache[i-2]
                 + (seq_t)2 * cache[i-3]
                 - (seq_t)4 * cache[i-4];
    }
    cached_until = n;
    return cache[n];
}
