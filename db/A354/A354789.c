/*
 * A354789  a(2*n) = 9*2^n - 7, a(2*n+1) = 3*2^(n+2) - 7.
 * https://oeis.org/A354789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 9*2^n - 7, a(2*n+1) = 3*2^(n+2) - 7
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 57, u64 n <= 121, u128 n <= 249,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    idx_t m;
    seq_t pow2, result;
    if ((n & 1) == 0) {             // even index
        m = n >> 1;                  // n/2
        pow2 = pow_uint(2, m);        // 2^m
        result = (seq_t)9 * pow2 - (seq_t)7;
    } else {                         // odd index
        m = (n - 1) >> 1;            // (n-1)/2
        pow2 = pow_uint(2, m + 2);    // 2^(m+2)
        result = (seq_t)3 * pow2 - (seq_t)7;
    }
    return result;
}
