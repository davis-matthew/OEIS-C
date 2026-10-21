/*
 * A287189  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 251", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1)-1 if n is odd, a(n) = (2^m-1) * 2^(n+1-m) if n is
 *            even, where m is the largest power of two such that 2^k-2 <= n
 *            (equivalently, m = 1 << (floor_log2_uint64(n+2)-1))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n & 1) /* odd n */
        return pow2(n + 1) - 1;

    /* even n */
    idx_t l = floor_log2_uint64(n + 2);   /* l >= 1 for n >= 0 */
    idx_t shift = l - 1;                  /* power-of-two exponent shift */
    idx_t m = ((idx_t)1) << shift;        /* number of leading 1 bits */

    seq_t high = pow2(m) - 1;             /* 2^m - 1 */
    idx_t low_shift = n + 1 - m;          /* remaining shift for the tail zeros */
    seq_t low = pow2(low_shift);          /* 2^(n+1-m) */

    return high * low;
}
