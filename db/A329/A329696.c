/*
 * A329696  Number of excursions of length n with Motzkin-steps avoiding the
 *          consecutive steps UH, HU, HD and DH.
 * https://oeis.org/A329696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = 1 for n odd; a(0)=1; a(n) = catalan(n/2)+1 for n>0 even
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n & 1)
        return (seq_t)1;

    idx_t k = n >> 1;          /* k = n/2 */
    return catalan(k) + (seq_t)1;
}
