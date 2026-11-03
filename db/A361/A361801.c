/*
 * A361801  Number of nonempty subsets of {1..n} with median n/2.
 * https://oeis.org/A361801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A079309(floor(n/2))
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t k = n >> 1;          /* floor(n/2) */
    if (k == 0) return (seq_t)0;   /* a(0) = 0 */

    seq_t res = (seq_t)1;      /* a(2) = 1 corresponds to k = 1 */
    for (idx_t i = 2; i <= k; ++i) {
        res += binomial(2 * i - 1, i);   /* add C(2i-1, i) */
    }
    return res;
}
