/*
 * A354788  a(2*k) = 3*2^k - 3, a(2*k+1) = 2^(k+2) - 3.
 * https://oeis.org/A354788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*k) = 3*2^k - 3, a(2*k+1) = 2^(k+2) - 3
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 61, u64 n <= 125,
 *            u128 n <= 253, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {            /* even index: n = 2*k */
        idx_t k = n >> 1;          /* k = n/2 */
        seq_t p = pow2(k);         /* 2^k   */
        return p * (seq_t)3 - (seq_t)3;
    } else {                       /* odd index: n = 2*k+1 */
        idx_t k = n >> 1;          /* k = (n-1)/2 */
        seq_t p = pow2(k + 2);     /* 2^(k+2) */
        return p - (seq_t)3;
    }
}
