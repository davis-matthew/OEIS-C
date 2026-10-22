/*
 * A318366  a(n) = Sum_{d|n} bigomega(d)*bigomega(n/d).
 * https://oeis.org/A318366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} bigomega(d) * bigomega(n/d)
 * Range:     u8 n <= 719, u16 n <= 9609, u32 n <= 9609, u64 n <= 9609,
 *            u128 n <= 9609, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1) return 0;

    seq_t res = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            seq_t ow_d  = bigomega(d);
            seq_t ow_q  = bigomega(n / d);
            seq_t prod  = ow_d * ow_q;
            if (d == n / d) {
                res += prod;          /* perfect square: count once */
            } else {
                res += prod * 2;       /* d and n/d are distinct */
            }
        }
    }
    return res;
}
