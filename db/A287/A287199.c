/*
 * A287199  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 259", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(n) = 2^n for n>1 and even, a(n) = 2^(n-1) - 1
 *            for n odd
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper pow2(idx_t) returning 2^exp in seq_t. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 3;
    if ((n & 1ULL) == 0)            /* even n > 1 */
        return pow2(n);
    else                            /* odd n */
        return pow2(n - 1) - 1;
}
