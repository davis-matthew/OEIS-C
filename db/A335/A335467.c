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
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/*-----------------------------------------------------------------------
 * pattern_avoids
 *
 * Checks whether the composition associated with index k avoids the
 * pattern (1,2,1).  The composition at index k is obtained by:
 *   - n := bit length of k (0 if k==0)
 *   - offset := k - 2^(n-1)  (position within the block of sum n)
 *   - Interpret 'offset' as a binary mask of cuts of length n-1:
 *        bit (j) = 1  ⇔  there is a cut after position j+1
 *   - Build the list of parts by scanning positions 1..n.
 *
 * The pattern (1,2,1) demands a subsequence of three parts where the
 * first and third parts are equal and strictly smaller than the middle.
 * The test enumerates all triples i < j < k for which
 *   parts[i] == parts[k]  &&  parts[i] < parts[j].
 * If such a triple exists, the pattern is present and the composition
 * does not avoid it.
 *-----------------------------------------------------------------------*/
static bool pattern_avoids(idx_t k)
{
    /* Handle the empty composition (k == 0). */
    if (k == 0)
        return true;

    /* Determine n = bit length of k (number of bits needed to represent k). */
    int n = 0;
    idx_t temp = k;
    while (temp) {
        ++n;
        temp >>= 1;
    }

    /* Offset within the block of compositions of sum n. */
    idx_t offset = k - (1ULL << (n - 1));

    /* Build the composition parts into a small local array.  n <= 64. */
    seq_t parts[64];
    size_t len = 0;
    seq_t cur = 0;

    for (int pos = 1; pos <= n; ++pos) {
        ++cur; /* part length increases as we walk through positions */

        /* A cut occurs after position 'pos' if either: */
        /*   - pos == n           (end of composition)  */
        /*   - the corresponding bit in 'offset' is set. */
        if (pos == n || (offset & (1ULL << (pos - 1)))) {
            parts[len++] = cur;
            cur = 0;
        }
    }

    /* Simple O(len³) test for the pattern (1,2,1).  The lengths are tiny
     * (say <= 64), so the cost is negligible for the required ranges. */
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i + 1; j < len; ++j) {
            if (parts[j] > parts[i]) {
                for (size_t k2 = j + 1; k2 < len; ++k2) {
                    if (parts[i] == parts[k2])
                        return false;
                }
            }
        }
    }
    return true;
}

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
            cache = mem_resize(seq_t, cache, cache_cap);
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
