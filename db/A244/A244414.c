/*
 * A244414  Remove highest power of 6 from n.
 * https://oeis.org/A244414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n / 6^A122841(n) (remove the highest power of 6 from n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute exponent of 2 in n */
    idx_t e2 = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        e2++;
        tmp >>= 1;
    }

    /* compute exponent of 3 in n */
    idx_t e3 = 0;
    tmp = n;
    while (tmp % 3ULL == 0) {
        e3++;
        tmp /= 3ULL;
    }

    /* exponent of 6 dividing n is the minimum of those two */
    idx_t e = e2 < e3 ? e2 : e3;

    /* divide n by 6^e */
    seq_t res = (seq_t)n;
    for (idx_t i = 0; i < e; ++i) {
        res /= 6;
    }

    return res;
}
