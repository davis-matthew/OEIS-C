/*
 * A396792  Maximum number of parts K in a partition of n such that there
 *          exists a part size p whose multiplicity is exactly p*K/100.
 * https://oeis.org/A396792
 *
 * Offset:    19    (a(19) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the maximum number of parts K in a partition of n such that
   there exists a part size p whose multiplicity is exactly p*K/100.
   Implements the definition directly: for each p≥2 we search all
   feasible K (via t) and test whether a partition exists. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd_int.h"

/* Euclidean GCD for small integers */

seq_t a(idx_t n) {
    if (n < 19) return 0;            /* undefined below the offset */

    seq_t best = 0;
    for (int p = 2; p <= (int)n; ++p) {               /* p must be ≥2 */
        int d = gcd_int(p, 100);
        seq_t pprime = (seq_t)p / d;                 /* p' = p/d  */
        seq_t hundredprime = (seq_t)100 / d;         /* 100' = 100/d */
        seq_t F = hundredprime + (seq_t)(p - 1) * pprime;   /* denominator */

        if (F == 0) continue;   /* safety, never happens for p≥2 */

        seq_t max_t = n / F;     /* largest t such that t*F ≤ n */

        for (seq_t t = 1; t <= max_t; ++t) {
            seq_t K   = hundredprime * t;        /* total number of parts */
            seq_t m   = pprime * t;             /* multiplicity of part p */
            seq_t s   = K - m;                  /* number of non‑p parts   */
            seq_t r   = (seq_t)n - m * (seq_t)p;/* sum of non‑p parts      */

            if (s == 0) continue;              /* impossible for p≥2 */

            /* Feasibility test for the remaining sum r
               using s parts not equal to p.            */
            if (s == 1) {
                if (r == (seq_t)p) continue;   /* the sole part would be p */
            } else {
                /* Special case: p==2 forbids a delta of 1.
                   While distributing the extra sum (r-s) among the
                   parts, an extra of 1 cannot be used. */
                if (p == 2) {
                    seq_t extra = r - s;        /* r - (s‑1) == delta of last part */
                    if (extra == 1) continue;   /* impossible */
                }
            }

            if (K > best) best = K;
        }
    }
    return best;
}
