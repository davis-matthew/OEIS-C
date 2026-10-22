/*
 * A376651  Points of upward concavity in the sequence of composite numbers
 *          (A002808).
 * https://oeis.org/A376651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the (n‑th) index k where the first difference between
 *            consecutive composite numbers strictly increases; in other
 *            words, a(n) = i – 1 for each i such that (c_{i} – c_{i‑1}) >
 *            (c_{i‑1} – c_{i‑2}), with c_m the m‑th composite number (c_1 =
 *            4)
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; n≥1 in normal use */

    /* Cache of already computed terms of the sequence. */
    static seq_t *ascents = NULL;
    static size_t cache_len  = 0;          /* number of terms stored */
    static size_t cache_cap  = 0;          /* allocated capacity */

    /* State variables preserved between calls to generate the sequence on
       demand.  We keep the last two composites seen, the previous
       first‑difference value, the index of the current first difference
       (diff_index), and the integer we are currently testing (cur). */
    static idx_t last1   = 0;   /* composite seen two steps ago */
    static idx_t last2   = 0;   /* composite seen one step ago   */
    static idx_t prev_diff = 0; /* previous first difference    */
    static idx_t diff_index = 0;/* number of first differences seen */
    static idx_t cur = 4;       /* integer being examined */

    /* Generate terms until we have at least n values cached. */
    while (cache_len < (size_t)n)
    {
        /* Find the next composite number ≥ cur. */
        while (is_prime(cur)) ++cur;
        idx_t current = cur;          /* composite value */

        if (last1 == 0) {
            /* First composite encountered. */
            last1 = current;
        } else if (last2 == 0) {
            /* Second composite; compute the first difference. */
            last2 = current;
            diff_index = 1;                /* first difference has index 1 */
            prev_diff = last2 - last1;     /* d_1 */
        } else {
            /* Compute the new first difference d_{diff_index+1}. */
            idx_t curr_diff = current - last2;
            ++diff_index;                  /* advance to this difference */

            if (curr_diff > prev_diff) {
                /* A strict ascent has been found; store (diff_index – 1). */
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 32;
                    seq_t *tmp = (seq_t *)realloc(ascents, cache_cap * sizeof(seq_t));
                    if (!tmp) abort();      /* allocation failure */
                    ascents = tmp;
                }
                ascents[cache_len++] = (seq_t)(diff_index - 1);
            }

            /* Shift state for the next composite. */
            prev_diff = curr_diff;
            last1 = last2;
            last2 = current;
        }

        ++cur;                            /* advance to the next integer */
    }

    return ascents[n - 1];
}
