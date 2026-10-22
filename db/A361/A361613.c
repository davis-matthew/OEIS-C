/*
 * A361613  The number of magic quad squares that can be formed using cards
 *          from Quads-2^n deck.
 * https://oeis.org/A361613
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n(2^n - 1)(2^n - 2)(2^n - 4)(2^n - 8)(10 + 85(2^n - 16)
 *            + 43(2^n - 16)(2^n - 32) + (2^n - 16)(2^n - 32)(2^n - 64))
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n < 4) return 0;

    seq_t t   = pow2(n);                // 2^n
    seq_t t1  = t - 1;                  // 2^n - 1
    seq_t t2  = t - 2;                  // 2^n - 2
    seq_t t4  = t - 4;                  // 2^n - 4
    seq_t t8  = t - 8;                  // 2^n - 8

    seq_t t16 = t - 16;
    seq_t t32 = t - 32;
    seq_t t64 = t - 64;

    seq_t inner = 10
        + 85 * t16
        + 43 * t16 * t32
        + t16 * t32 * t64;

    seq_t result = t * t1 * t2 * t4 * t8 * inner;
    return result;
}
