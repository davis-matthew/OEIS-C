/*
 * A242026  Number of non-palindromic n-tuples of 4 distinct elements.
 * https://oeis.org/A242026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 4^ceil(n/2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* ceil(n/2) is (n+1)/2 for unsigned integers */
    idx_t half = (n + 1) / 2;
    seq_t full = pow4(n);
    seq_t halfpow = pow4(half);
    return full - halfpow;
}
