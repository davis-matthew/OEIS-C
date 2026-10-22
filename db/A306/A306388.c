/*
 * A306388  a(n) is a decimal number k having a length n binary expansion
 *          which encodes, from left to right at digit j, the coprimality (0)
 *          or non-coprimality (1) of j to n, for 1 < j <= n, except for the
 *          first digit, which is always 1.
 * https://oeis.org/A306388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary number of length n with first bit 1, where bit j
 *            (2 ≤ j ≤ n) is 0 if gcd(j,n)=1 else 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed */
#include "../utils/gcd_idx.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;        /* first bit always 1 */
    for (idx_t j = 2; j <= n; ++j) {
        idx_t g  = gcd_idx(j, n);
        int    bit = (g > 1) ? 1 : 0;
        val = (val << 1) | (seq_t)bit;
    }
    return val;
}
