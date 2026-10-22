/*
 * A309575  Expansion of Product_{k>=1} (1 - (x*(1 + x))^k).
 * https://oeis.org/A309575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{m} g_m · binom(m, n-m) where g_m = (-1)^t if m =
 *            t(3t±1)/2, else 0. This follows from the pentagonal number
 *            theorem applied to G(x) = ∏_{k≥1} (1 - (x+x²)^k) = ∏_{k≥1} (1 -
 *            y^k) with y=x+x²
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   binom_pos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom_pos.h"

seq_t a(idx_t n) {
    seq_t total = 0;
    if (n == 0) return 1;          /* the term a(0) = 1 */

    /* iterate over all pentagonal numbers m <= n */
    for (idx_t t = 0;; ++t) {
        /* m1 = t(3t-1)/2, m2 = t(3t+1)/2 */
        idx_t m1 = t * (3 * t - 1) / 2;
        if (m1 > n) break;         /* larger pentagonal numbers will exceed n */
        seq_t sign = (t & 1U) ? -1 : 1;   /* (-1)^t */

        if (m1 > 0) {              /* skip m1=0 since we handled n==0 */
            if (n >= m1 && n <= 2 * m1) {
                idx_t k = n - m1;  /* k ∈ [0, m1] */
                total += sign * binom_pos(m1, k);
            }
        }

        /* m2 is distinct from m1 except for t=0, which we skip */
        idx_t m2 = t * (3 * t + 1) / 2;
        if (m2 > n) continue;
        if (m2 > 0) {
            if (n >= m2 && n <= 2 * m2) {
                idx_t k = n - m2;
                total += sign * binom_pos(m2, k);
            }
        }
    }
    return total;
}
