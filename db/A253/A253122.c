/*
 * A253122  Number of length n+2 0..1 arrays with the sum of medians of
 *          adjacent triples multiplied by some arrangement of +-1 equal to
 *          zero.
 * https://oeis.org/A253122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+1) if n is odd Formula: a(n) = 2^(n/2+1) + 2^(n+1)
 *            if n is even
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* not in the defined range, safe fallback */
    if (n & 1U)                       /* odd */
        return pow2(n + 1);
    /* even */
    return pow2((n >> 1) + 1) + pow2(n + 1);
}
