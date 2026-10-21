/*
 * A291097  a(n) = 2^n*(n/8 + 1) - n.
 * https://oeis.org/A291097
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (n/8 + 1) - n, for n ≥ 2
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* sequence is defined only for n ≥ 2; return 0 for n < 2 (unused) */
    if (n < 2) return 0;
    if (n == 2) {
        /* a(2) = 3 */
        return (seq_t)3;
    }

    /* For n ≥ 3, 2^n is divisible by 8, so we can shift right first to avoid overflow */
    seq_t two = pow2(n);                     /* 2^n  */
    seq_t div_by_8 = two >> 3;               /* 2^(n-3) */
    seq_t factor = ((seq_t)n + 8);             /* n + 8  */
    seq_t result = div_by_8 * factor;        /* 2^(n-3) * (n+8) = 2^n * (n+8)/8 */
    result -= n;                            /* subtract n */
    return result;
}
