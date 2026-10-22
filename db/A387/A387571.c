/*
 * A387571  Number of matchings in the n-double cone graph.
 * https://oeis.org/A387571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1)-5*a(n-2)-5*a(n-3)+5*a(n-4)-a(n-5), with base
 *            values a(1)=3, a(2)=21, a(3)=64, a(4)=217, a(5)=671
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t base_vals[5] = {3, 21, 64, 217, 671};

/* Cache of computed terms: cache[0] holds a(1) */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of elements currently in cache */

/* Return a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n = 0 */

    /* Direct return for the five initial values */
    if (n <= 5) return base_vals[n-1];

    /* Grow cache if necessary */
    if (!cache || cache_size < n) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {                   /* allocation failed */
            exit(EXIT_FAILURE);
        }
        cache = tmp;

        /* If this is the first allocation, insert the base values */
        if (cache_size == 0) {
            for (idx_t i = 0; i < 5; ++i) cache[i] = base_vals[i];
        }
        /* If we've grown but are still below the fifth value, finish it */
        else if (cache_size < 5) {
            for (idx_t i = cache_size; i < 5 && i < new_size; ++i)
                cache[i] = base_vals[i];
        }

        /* Compute the missing terms using the recurrence */
        idx_t start = (cache_size > 5 ? cache_size : 5);
        for (idx_t i = start; i < new_size; ++i) {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t a5 = cache[i-5];
            cache[i] = 5*a1 - 5*a2 - 5*a3 + 5*a4 - a5;
        }
        cache_size = new_size;
    }

    return cache[n-1];
}
