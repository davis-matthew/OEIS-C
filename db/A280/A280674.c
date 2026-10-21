/*
 * A280674  Number of 2 X n 0..2 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors and with new values
 *          introduced in order 0 sequentially upwards.
 * https://oeis.org/A280674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 6*a(n-2) - 11*a(n-3) - 7*a(n-4) + 4*a(n-5) for
 *            n > 6, with a(1)=2, a(2)=11, a(3)=59, a(4)=338, a(5)=1917,
 *            a(6)=10893
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static const seq_t init_vals[6] = {
    (seq_t)2,
    (seq_t)11,
    (seq_t)59,
    (seq_t)338,
    (seq_t)1917,
    (seq_t)10893
};

static seq_t *cache = NULL;   /* 1‑based cache of computed terms */
static size_t cache_len = 0; /* highest index stored in cache */
static size_t cache_cap = 0; /* allocated capacity of cache */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined, safe */

    /* Return initial values directly */
    if (n <= 6)
        return init_vals[n - 1];

    /* Initialize cache with the first 6 terms if needed */
    if (cache == NULL) {
        cache_cap = 6;
        cache_len = 6;
        cache = seq_alloc(cache_cap);   /* allocate */
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init_vals[i];
    }

    /* Grow cache capacity if necessary */
    if ((size_t)n > cache_len) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n)
            new_cap <<= 1;               /* grow by factor of 2 */

        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;

        /* Compute missing terms using the recurrence */
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            size_t idx = i - 1;           /* zero‑based index in array */

            seq_t term = ((seq_t)5 * cache[idx - 1])
                       + ((seq_t)6 * cache[idx - 2])
                       - ((seq_t)11 * cache[idx - 3])
                       - ((seq_t)7 * cache[idx - 4])
                       + ((seq_t)4 * cache[idx - 5]);

            cache[idx] = term;
        }
        cache_len = n;
    }

    return cache[(size_t)n - 1];
}
