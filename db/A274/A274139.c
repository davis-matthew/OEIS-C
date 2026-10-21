/*
 * A274139  a(n) = 2^A000265(n) = 2^numerator(n/2^n), a sequence related to
 *          Oresme numbers.
 * https://oeis.org/A274139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n/2^valuation(n,2))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1, but handle n==0 defensively. */
    if (n == 0)
        return 0;

    /* Extract the odd part of n: divide out all factors of 2. */
    idx_t odd = n;
    while ((odd & 1ULL) == 0)
        odd >>= 1;

    /* a(n) = 2^odd_part(n) */
    return pow_uint(2, odd);
}
