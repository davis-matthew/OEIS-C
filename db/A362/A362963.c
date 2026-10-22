/*
 * A362963  Number of semimagic quads squares that can be formed using cards
 *          from Quads-2^n deck, where the first row and column are fixed.
 * https://oeis.org/A362963
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 112 + 2823*(2^n-16) + 2531*(2^n-16)*(2^n-32) +
 *            159*(2^n-16)*(2^n-32)*(2^n-64) +
 *            (2^n-16)*(2^n-32)*(2^n-64)*(2^n-128)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;   /* values for n < 4 are not defined */

    seq_t two_n = pow2(n);
    seq_t t16   = two_n - (seq_t)16;
    seq_t t32   = two_n - (seq_t)32;
    seq_t t64   = two_n - (seq_t)64;
    seq_t t128  = two_n - (seq_t)128;

    seq_t res = (seq_t)112;
    res += (seq_t)2823 * t16;
    res += (seq_t)2531 * t16 * t32;
    res += (seq_t)159   * t16 * t32 * t64;
    res += t16 * t32 * t64 * t128;

    return res;
}
