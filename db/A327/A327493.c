/*
 * A327493  a(n) = 2^A327492(n).
 * https://oeis.org/A327493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{e(n)} where if n = 2m e(n) = 4m - popcount(m) if n =
 *            2m+1 e(n) = 4m + 2 - popcount(m)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t m = n >> 1;                                /* floor(n/2) */
    unsigned int pop = __builtin_popcountll((unsigned long long)m);
    idx_t e = 4 * m + ((n & 1) ? 2 : 0) - pop;       /* exponent of 2 */
    return pow2(e);
}
