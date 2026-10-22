/*
 * A303611  a(n) = (-1 - (-2)^(n-2)) mod 2^n.
 * https://oeis.org/A303611
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2 + (-1)^n)*2^(n-2) - 1, offset=2
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* defined for n >= 2; the function will be called only with n ≥ 2 */
    seq_t p = pow2(n - 2);          /* compute 2^(n‑2) */
    if (n & 1) {                    /* odd n  →  2^(n-2) - 1 */
        return p - 1;
    } else {                        /* even n → 3*2^(n-2) - 1 */
        return p * 3 - 1;
    }
}
