/*
 * A266069  Decimal representation of the n-th iteration of the "Rule 3"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*4^n - 3*2^((n-1)/2) - 1 for odd n; a(n) = 2^(n/2) for
 *            even n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Even indices */
    if ((n & 1) == 0) {
        /* a(2k) = 2^k */
        return pow2(n / 2);
    }

    /* Odd indices */
    /* 4^n = 2^(2n) */
    seq_t pow4n = pow2((idx_t)(2 * n));
    /* 2^((n-1)/2) */
    seq_t powhalf = pow2((n - 1) / 2);

    /* a(2k+1) = 2*4^n - 3*2^((n-1)/2) - 1 */
    return pow4n * 2 - powhalf * 3 - (seq_t)1;
}
