/*
 * A326699  Numerator of the average position of a 1 in the reversed binary
 *          expansion of n.
 * https://oeis.org/A326699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) = numerator of the average position of a 1 in the reversed
 *            binary expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t       temp  = n;
    idx_t       pos   = 1;          /* positions start at 1 for the LSB */
    seq_t       sum   = 0;          /* sum of the positions of the 1‑bits */
    seq_t       count = 0;          /* number of 1‑bits */

    while (temp) {
        if (temp & ((idx_t)1)) {
            sum   += (seq_t)pos;
            count += (seq_t)1;
        }
        temp >>= 1;
        pos++;
    }

    /* n ≥ 1 guarantees count > 0 */
    seq_t g = gcd(sum, count);
    return sum / g;
}
