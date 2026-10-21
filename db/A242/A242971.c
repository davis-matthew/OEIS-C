/*
 * A242971  Alternate n+1, 2^n.
 * https://oeis.org/A242971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+1) mod 2) * (n/2 + 1) + (n mod 2) * 2^((n-1)/2)
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 256, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        /* n even: a(n) = n/2 + 1 */
        return (seq_t)(n / 2 + 1);
    } else {
        /* n odd: a(n) = 2^((n-1)/2) */
        return pow2((n - 1) / 2);
    }
}
