/*
 * A284002  a(n) = A072411(A283477(n)).
 * https://oeis.org/A284002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(1,2,…,A283477(n)) where A283477(n) = popcount(n)
 *            (number of 1-bits in n) and A072411(k) = lcm(1,2,…,k)
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute popcount of n */
    idx_t pop = 0;
    idx_t tmp = n;
    while (tmp) {
        tmp &= (tmp - 1);
        ++pop;
    }

    /* lcm(1 … pop) – for pop = 0 the loop is empty, result stays 1 */
    seq_t res = 1;
    for (idx_t i = 2; i <= pop; ++i) {
        seq_t ii = (seq_t)i;
        seq_t g = gcd(res, ii);
        /* res = lcm(res, i) = res / gcd(res, i) * i */
        res = (seq_t)((seq_t)res / g) * ii;
    }

    return res;
}
