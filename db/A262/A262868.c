/*
 * A262868  Number of squarefree numbers appearing among the larger parts of
 *          the partitions of n into two parts.
 * https://oeis.org/A262868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} mu(n-i)^2, where mu is the Möbius
 *            function A008683
 * Range:     u8 n <= 831, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t half   = n >> 1;   /* floor(n/2) */
    for (idx_t i = 1; i <= half; ++i)
    {
        seq_t mu = mobius(n - i);
        if (mu != 0)
            count += 1;
    }
    return count;
}
