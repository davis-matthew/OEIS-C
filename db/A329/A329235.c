/*
 * A329235  Number of nonequivalent symmetric sets whose translations cover
 *          {1..n}.
 * https://oeis.org/A329235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 33, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   count_symmetric_sets
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A329235
    a(n) = number of non‑equivalent symmetric subsets of {1..n} whose
    translations (restricted to {1..n}) cover the whole set.
    The implementation enumerates the offset sets of a minimal
    representative (min element = 1).  For a given d = max‑min,
    let L = n – d.  A symmetric offset set is encoded by an
    increasing sequence
          0 = s0 < s1 < … < sk ≤ ⌊d/2⌋
    (the symmetric partners are determined automatically).  The
    translation covering condition is equivalent to

          0 = s0,
          si – s(i−1) ≤ L  for i≥1,
          d – 2·sk ≤ L.

    The count f(d,L) of such sequences is computed by a simple
    DP which only uses additions/subtractions.  a(n) = Σ_{d=0}^{n−1} f(d,n−d).
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/count_symmetric_sets.h"

seq_t a(idx_t n)
{
    if (n == 0) {            /* sequence is defined for n≥1; safe guard */
        return (seq_t)0;
    }

    /* Allocate DP arrays once (max size n/2 + 2) */
    idx_t max_t = n / 2 + 1;
    seq_t *dp   = (seq_t*)malloc(max_t * sizeof(seq_t));
    seq_t *pref = (seq_t*)malloc(max_t * sizeof(seq_t));
    if (!dp || !pref) {
        return (seq_t)0; /* allocation failure */
    }

    seq_t total = (seq_t)0;

    for (idx_t d = 0; d < n; ++d) {
        idx_t L = n - d;            /* allowed shift length */
        total += count_symmetric_sets(d, L, dp, pref);
    }

    free(dp);
    free(pref);
    return total;
}
