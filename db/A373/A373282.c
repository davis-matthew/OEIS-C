/*
 * A373282  Expansion of Sum_{k>=0} x^(6^k) / (1 - 6*x^(6^k)).
 * https://oeis.org/A373282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^(n-1) if n mod 6 ≠ 0; else 6^(n-1)+a(n/6); a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    seq_t val = seq_pow((seq_t)6, n-1);      /* 6^(n-1) */
    if (n % 6 == 0) {
        val += a(n/6);                       /* recursion for n/6 */
    }
    return val;
}
