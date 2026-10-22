/*
 * A300725  Möbius transform of A053645(n), distance to the largest power of 2
 *          less than or equal to n.
 * https://oeis.org/A300725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = Sum_{d|n} μ(n/d) * (d - 2^⌊log2 d⌋)
 * Range:     i8 n <= 386, i16 n <= 9465, i32 n <= 9465, i64 n <= 9465,
 *            i128 n <= 9465, big any n
 * Helpers:   floor_log2_uint64, mu_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* μ(k) is the Möbius function, d - 2^⌊log2 d⌋ is the distance of d
   to the largest power of 2 less than or equal to d. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/mu_simple.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t k1 = n / d1;
            int mu_k1 = mu_simple(k1);
            seq_t dist1 = (seq_t)d1 - (seq_t)((uint64_t)1 << floor_log2_uint64(d1));
            sum += (seq_t)mu_k1 * dist1;

            if (d1 != k1) {
                idx_t d2 = k1;
                idx_t k2 = d1;       /* k2 = n/d2 = d1 */
                int mu_k2 = mu_simple(k2);
                seq_t dist2 = (seq_t)d2 - (seq_t)((uint64_t)1 << floor_log2_uint64(d2));
                sum += (seq_t)mu_k2 * dist2;
            }
        }
    }
    return sum;
}
