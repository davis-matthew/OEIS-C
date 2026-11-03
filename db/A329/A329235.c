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
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* Count of symmetric offset sets for given d and L (L = n-d) */
static seq_t count_symmetric_sets(idx_t d, idx_t L, seq_t *dp, seq_t *pref)
{
    idx_t t = d / 2;               /* floor(d/2) */
    dp[0] = (seq_t)1;
    pref[0] = (seq_t)1;

    for (idx_t j = 1; j <= t; ++j) {
        seq_t sum = pref[j - 1];
        if (j > L) {               /* subtract sum of excluded prefix */
            idx_t sub = j - L - 1;
            sum -= pref[sub];
        }
        dp[j] = sum;
        pref[j] = pref[j - 1] + sum;
    }

    seq_t total = (seq_t)0;
    seq_t d_val = (seq_t)d;
    seq_t L_val = (seq_t)L;
    for (idx_t j = 0; j <= t; ++j) {
        /* condition: d - 2*j <= L   <=>   d <= L + 2*j */
        seq_t two_j = (seq_t)2 * (seq_t)j;
        if (d_val <= L_val + two_j) {
            total += dp[j];
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) {            /* sequence is defined for n≥1; safe guard */
        return (seq_t)0;
    }

    /* Allocate DP arrays once (max size n/2 + 2) */
    idx_t max_t = n / 2 + 1;
    seq_t *dp   = mem_new(seq_t, max_t);
    seq_t *pref = mem_new(seq_t, max_t);
    if (!dp || !pref) {
        return (seq_t)0; /* allocation failure */
    }

    seq_t total = (seq_t)0;

    for (idx_t d = 0; d < n; ++d) {
        idx_t L = n - d;            /* allowed shift length */
        total += count_symmetric_sets(d, L, dp, pref);
    }

    mem_free(dp);
    mem_free(pref);
    return total;
}
