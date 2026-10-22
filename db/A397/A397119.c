/*
 * A397119  Size of clusters of star triples of Family E in the Spectre
 *          aperiodic monotile tiling.
 * https://oeis.org/A397119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 15*a(n-2) + a(n-3) + 12 (n > 3) a(1) = 4, a(2)
 *            = 25, a(3) = 147 (Offset = 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Handle requests below the offset in a permissive way */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Static cache ------------------------------------------------------- */
    static seq_t *cache = NULL;  /* array of terms, index 0 unused */
    static size_t cap   = 0;     /* number of allocated elements */
    static idx_t ready  = 0;     /* highest index that has a valid value */

    /* First time: allocate four slots (0…3) and initialise */
    if (cache == NULL) {
        cap = 4;
        cache = seq_alloc_new_array(cap);
        if (!cache) {
            return (seq_t)0;    /* allocation failure guard */
        }
        cache[0] = (seq_t)0;    /* unused slot */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)25;
        cache[3] = (seq_t)147;
        ready   = 3;
    }

    /* Grow the cache if needed ------------------------------------------- */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap) {
            new_cap *= 2;
        }

        seq_t *tmp = seq_alloc_new_array(new_cap);
        if (!tmp) {
            return (seq_t)0;    /* allocation failure guard */
        }

        /* Copy existing values */
        for (size_t i = 0; i <= (size_t)ready; ++i) {
            tmp[i] = cache[i];
        }

        /* Free the old cache */
        free_seq_array(cache);
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n ------------------------------------- */
    for (idx_t i = ready + 1; i <= n; ++i) {
        if (i <= 3) {
            continue;   /* already initialised */
        }
        seq_t term = (seq_t)12;
        term += cache[i - 1] * (seq_t)3;
        term += cache[i - 2] * (seq_t)15;
        term += cache[i - 3];
        cache[i] = term;
    }

    ready = n;
    return cache[(size_t)n];
}
