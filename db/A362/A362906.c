/*
 * A362906  Number of n element multisets of length 3 vectors over GF(2) that
 *          sum to zero.
 * https://oeis.org/A362906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+7,7)/8 for odd n; a(n) =
 *            (binomial(n+7,7)+7*binomial(n/2+3,3))/8 for even n
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 103, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n & 1) {  /* odd n */
        seq_t t = binomial(n + 7, 7);
        return t / 8;
    } else {      /* even n */
        seq_t t = binomial(n + 7, 7);
        seq_t s = binomial((n >> 1) + 3, 3);
        t = t + s * 7;
        return t / 8;
    }
}
