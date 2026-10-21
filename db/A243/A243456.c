/*
 * A243456  a(n+6) = 6*a(n+4) - 12*a(n+2) + 8*a(n), a(0)..a(5) = 8,0,9,0,8,0.
 * https://oeis.org/A243456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 0, if n is odd * a(n) = (n - 8)^2 * 2^((n - 6)/2), if n
 *            is even * * Derived from the OEIS entry: a(n) = (n-8)^2 *
 *            2^((n-6)/2) (n even) * For odd n the term is zero
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 49, u64 n <= 107,
 *            u128 n <= 231, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n & 1) return 0;                    // odd indices

    int64_t half  = (int64_t)n / 2;          // n/2, fits in 64‑bit for the test range
    int64_t exp   = half - 3;                // exponent for the power of 2
    int64_t diff  = (int64_t)n - 8;          // n – 8 (may be negative)
    seq_t square  = (seq_t)(diff * diff);    // (n-8)^2

    if (exp >= 0) {
        seq_t p = pow2((idx_t)exp);          // 2^exp
        return p * square;                   // multiply the two big integers
    } else {
        uint64_t shift = (uint64_t)(-exp);    // amount to right‑shift
        return square >> shift;              // divide by 2^(-exp)
    }
}
