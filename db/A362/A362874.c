/*
 * A362874  Number of strongly magic quad squares that can be formed using
 *          cards from Quads-2^n deck.
 * https://oeis.org/A362874
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2^n * (2^n - 1) * (2^n - 2) * (2^n - 4) * (2^n - 8) for n
 *            ≥ 4
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 4)                /* behavior below offset does not matter */
        return 0;

    seq_t p   = pow2(n);                     /* p = 2^n                         */
    seq_t res = p * (p - 1);
    res = res * (p - 2);
    res = res * (p - 4);
    res = res * (p - 8);
    return res;
}
