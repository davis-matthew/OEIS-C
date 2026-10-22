/*
 * A320131  Number of integer solutions to a^2 + b^2 + 2*c^2 + 9*d^2 = n.
 * https://oeis.org/A320131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to a^2 + b^2 + 2*c^2 + 9*d^2
 *            = n Smoothed by counting signs separately
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _s_r
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t, if needed */
#include "../utils/_s_r.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    /* iterate over non‑negative d; multiply by 2 for d>0 to account for ±d */
    for (idx_t d = 0; d * d * 9 <= n; ++d) {
        idx_t d2 = d * d * 9;
        idx_t rem_d = n - d2;
        seq_t multD = (d == 0) ? 1 : 2;
        /* iterate over non‑negative c; multiply by 2 for c>0 */
        for (idx_t c = 0; c * c * 2 <= rem_d; ++c) {
            idx_t c2 = c * c * 2;
            idx_t rem_c = rem_d - c2;
            seq_t multC = (c == 0) ? 1 : 2;
            seq_t abCount = _s_r(rem_c);
            total += multD * multC * abCount;
        }
    }
    return total;
}
