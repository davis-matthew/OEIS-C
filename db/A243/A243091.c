/*
 * A243091  Least number k > n such that n concatenated with k is a perfect
 *          square.
 * https://oeis.org/A243091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the least integer k > n such that the decimal
 *            concatenation of n and k is a perfect square
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   concat_parts, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/concat_parts.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    idx_t k = n + 1;
    while (1)
    {
        seq_t cat = concat_parts(n, k);    /* n concatenated with k */
        seq_t s   = floor_sqrt(cat);       /* floor(sqrt(cat))          */
        if (s * s == cat)                  /* perfect square test       */
            return (seq_t)k;
        ++k;
    }
}
