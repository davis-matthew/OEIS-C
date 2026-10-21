/*
 * A251861  Number of non-palindromic words (length n>0) over the alphabet of
 *          26 letters.
 * https://oeis.org/A251861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26^n - 26^ceil(n/2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Ensure we have a defined result even for n < 1. */
    if (n == 0) return 0;

    seq_t pow_n   = seq_pow((seq_t)26, n);
    seq_t pow_half = seq_pow((seq_t)26, (n + 1) / 2);
    return pow_n - pow_half;
}
