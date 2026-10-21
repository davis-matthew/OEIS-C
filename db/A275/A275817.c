/*
 * A275817  Least positive integer s such that an integer square k^2 lies
 *          between s^2*n and s^2*(n+1), with s^2*n < k^2 < s^2*(n+1).
 * https://oeis.org/A275817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS A275817.
   According to the definition:
   a(n) = the least positive integer s such that there exists an integer k
          with s^2 * n < k^2 < s^2 * (n+1).
   The algorithm checks successive s, using an integer square root to
   determine whether a square exists in the required interval.
*/

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    for (idx_t s = 1; ; ++s) {
        /* s^2 */
        seq_t s_sq = (seq_t)s * (seq_t)s;

        /* t = s^2 * (n + 1) */
        seq_t t = s_sq * ((seq_t)n + 1);

        /* largest integer k with k^2 < t, i.e. k = floor_sqrt(t - 1) */
        seq_t k = floor_sqrt(t - 1);

        /* check if k^2 > s^2 * n */
        if (k * k > s_sq * (seq_t)n) {
            return (seq_t)s;
        }
    }
}
