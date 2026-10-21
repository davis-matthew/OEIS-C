/*
 * A241864  Number of compositions of n such that the smallest part has
 *          multiplicity four.
 * https://oeis.org/A241864
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 38, u64 n <= 79, u128 n <= 165,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of compositions of n such that the smallest part has multiplicity four.
 * Computed by summing over the minimal part size s (1 ≤ s ≤ ⌊n/4⌋) and
 * over possible lengths k of the composition.  For k = 4 we count the
 * trivial composition with four equal parts; for k ≥ 5 we count
 * C(k,4) ways to choose the positions of the four minimal parts,
 * followed by C(n - k*s - 1, k - 5) ways to distribute the excess
 * over the remaining k−4 parts.  Formally:
 *
 * a(n) = Σ_{s=1}^{⌊n/4⌋} [
 *          1   if n = 4s
 *          + Σ_{k=5}^{kmax(s)} C(k,4) * C(n - k*s - 1, k - 5)
 *        ]
 * where kmax(s) = 4 + ⌊(n - 4s)/(s + 1)⌋.
 *
 * The function uses the utility binomial() for binomial coefficients.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n < 4) return 0;          /* offset is 4 */
    seq_t total = 0;
    idx_t max_s = n / 4;          /* largest possible minimal part */

    for (idx_t s = 1; s <= max_s; ++s) {
        /* maximum number of parts for this minimal value */
        idx_t kmax = 4 + (n - 4 * s) / (s + 1);
        if (kmax > n) kmax = n;   /* safety, though never needed */

        for (idx_t k = 4; k <= kmax; ++k) {
            idx_t m = n - k * s;  /* remaining amount after subtracting s from each part */

            if (k == 4) {
                if (m == 0) {
                    total += 1;   /* composition [s,s,s,s] */
                }
            } else { /* k >= 5 */
                if (m >= k - 4) {
                    /* binomial(k,4) * binomial(m-1, k-5) */
                    seq_t c1 = binomial(k, 4);
                    seq_t c2 = binomial(m - 1, k - 5);
                    total += c1 * c2;
                }
            }
        }
    }
    return total;
}
