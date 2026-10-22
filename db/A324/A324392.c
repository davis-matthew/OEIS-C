/*
 * A324392  a(n) is the number of divisors d of n such that A000120(d) divides
 *          n, where A000120(d) gives the binary weight of d.
 * https://oeis.org/A324392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n} [popcount(d) divides n]
 * Range:     u8 n <= 8917, u16 n <= 8917, u32 n <= 8917, u64 n <= 8917,
 *            u128 n <= 8917, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* 0 is outside the OEIS offset, but safe */ 
        return 0;

    seq_t count = 0;     /* result counter, stored in seq_t */

    for (idx_t d = 1; d <= n; ++d)          /* trial all divisors up to n */
    {
        if (n % d == 0)                     /* d actually divides n ? */
        {
            /* popcount of d, computed with idx_t bit ops */
            unsigned pc = 0;
            idx_t temp  = d;
            while (temp)                    /* count set bits of d */
            {
                pc += (unsigned)(temp & 1);
                temp >>= 1;
            }

            if (pc > 0 && n % pc == 0)      /* popcount divides n ? */
                ++count;                    /* count this divisor   */
        }
    }
    return count;                              /* return the final result */
}
