/*
 * A335467  Numbers k such that the k-th composition in standard order
 *          (A066099) avoids the pattern (1,2,1).
 * https://oeis.org/A335467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k such that the k‑th composition in the
 *            * standard order (graded reverse‑lexicographic) avoids the
 *            pattern (1,2,1). * This is computed by enumerating the
 *            compositions in standard order, * testing each one for the
 *            pattern, and collecting the k‑values that pass. * The routi...
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   pattern_avoids
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pattern_avoids.h"

/* Forward declaration of the helper that tests a composition for the pattern. */
static bool pattern_avoids(idx_t k);

/* External linkage function required by the build. */
seq_t a(idx_t n)
{
    /* Cache of previously computed good k's. */
    static size_t cache_len = 0;   /* number of entries filled */
    static size_t cache_cap = 0;   /* allocation capacity */
    static seq_t *cache = NULL;   /* array of good k's */
    static idx_t last_k = 0;      /* next k to test */

    if (n == 0)   /* not used in OEIS but keeps the function robust */
        return 0;

    /* Ensure that cache contains at least n entries. */
    if (cache_len < n) {
        /* Grow the cache if necessary. */
        if (cache_cap < (size_t)n) {
            cache_cap = (n > 64 ? n * 2 : 128);
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        while (cache_len < n) {
            /* Find the next good k starting from last_k. */
            while (!pattern_avoids(last_k))
                ++last_k;

            /* Store the good k. */
            cache[cache_len++] = (seq_t)last_k;
            ++last_k;   /* advance to next candidate */
        }
    }

    /* Return the (n‑1)th element (0‑based index). */
    return cache[n - 1];
}
