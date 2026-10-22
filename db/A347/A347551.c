/*
 * A347551  Number of minimum dominating sets in the 2n-crossed prism graph.
 * https://oeis.org/A347551
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^((n/2)+1) for n even; a(n) = 17*n*2^((n-3)/2) for n odd
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 48, u64 n <= 110, u128 n <= 236,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* offset is 2; values for n < 2 are undefined */
    if ((n & 1) == 0) {            /* even n */
        return pow2(((n >> 1) + 1));
    } else {                       /* odd n */
        seq_t mult = (seq_t)17 * (seq_t)n;
        return mult * pow2(((n - 3) >> 1));
    }
}
