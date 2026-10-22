/*
 * A352452  2^k appears in the binary expansion of a(n) iff 2^k appears in the
 *          binary expansion of n and k+1 does not divide n.
 * https://oeis.org/A352452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = bitwise OR of 2^k, where bit k of n is 1 and (k+1) does
 *            NOT divide n
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t temp = n;       /* iterate over set bits of n */
    idx_t k = 0;          /* current bit position (0‑based) */

    while (temp != 0) {
        if ((temp & 1) != 0) {
            /* include bit k if (k+1) does not divide n */
            if (n % (k + 1) != 0) {
                res |= pow2(k);
            }
        }
        temp >>= 1;
        ++k;
    }
    return res;
}
