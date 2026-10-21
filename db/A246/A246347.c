/*
 * A246347  Record values in A135141.
 * https://oeis.org/A246347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(2) = 2 for n >= 3: if n is even: a(n) = 2^(n/2 + 1) +
 *            2^(n/2 - 2) - 1 if n is odd: a(n) = 2^((n-1)/2 + 1) + 2^((n-1)/2
 *            - 1) - 1 The function uses pow2 from "../utils/pow2.h" to
 *            construct powers of two
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 61, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    if ((n & 1) == 0) {                 /* even n */
        idx_t k = n >> 1;               /* n/2 */
        seq_t term1 = pow2(k + 1);      /* 2^(k+1) */
        seq_t term2 = pow2(k - 2);      /* 2^(k-2)  (k>=2 here) */
        return term1 + term2 - (seq_t)1;
    } else {                            /* odd n */
        idx_t k = (n - 1) >> 1;         /* (n-1)/2 */
        seq_t term1 = pow2(k + 1);      /* 2^(k+1) */
        seq_t term2 = pow2(k - 1);      /* 2^(k-1)  (k>=1 here) */
        return term1 + term2 - (seq_t)1;
    }
}
