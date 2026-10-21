/*
 * A242278  Number of non-palindromic n-tuples of 3 distinct elements.
 * https://oeis.org/A242278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 3^ceil(n/2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* ceil(n/2) = (n + 1) / 2 for unsigned indices */
    idx_t half = (n + 1) / 2;
    seq_t powN = pow3(n);
    seq_t powHalf = pow3(half);
    return powN - powHalf;
}
