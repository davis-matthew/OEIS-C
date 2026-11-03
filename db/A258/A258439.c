/*
 * A258439  Powers of 3 alternating with powers of 2.
 * https://oeis.org/A258439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((5+(-1)^n)/2)^((2*n-1+(-1)^n)/4)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 81, u128 n <= 161,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 1000 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0) {          /* even n: a = 3^(n/2) */
        idx_t e = n >> 1;           /* n/2 */
        return ipow_u64_idx(3, e);
    } else {                        /* odd n:  a = 2^((n-1)/2) */
        idx_t e = (n - 1ULL) >> 1;  /* (n-1)/2 */
        return ipow_u64_idx(2, e);
    }
}
