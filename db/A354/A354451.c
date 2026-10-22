/*
 * A354451  Number of middle divisors of 2*n-1.
 * https://oeis.org/A354451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of m = 2*n-1 such that * m / d <= 2
 *            * d (counting d and its co-divisor separately * if d != m/d).
 *            This matches the OEIS definition of * number of middle divisors
 *            of an odd number
 * Range:     u8 n <= 32, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to the odd number whose middle divisors are counted */
    seq_t m = (seq_t)n * (seq_t)2 - (seq_t)1;
    seq_t root = floor_sqrt(m);
    seq_t count = 0;

    for (idx_t d = 1; (seq_t)d <= root; ++d)
    {
        if (m % (seq_t)d == 0)
        {
            seq_t e = m / (seq_t)d;           /* complementary divisor */
            if (e <= (seq_t)d * (seq_t)2)
            {
                if (e == (seq_t)d)
                    count += 1;              /* perfect square case */
                else
                    count += 2;              /* pair of distinct divisors */
            }
        }
    }
    return count;
}
