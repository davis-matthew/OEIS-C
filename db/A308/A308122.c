/*
 * A308122  Sum of the perimeters of all scalene triangles with perimeter n
 *          whose side lengths are squarefree.
 * https://oeis.org/A308122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{k=1..floor((n-1)/3)}
 *            Sum_{i=k+1..floor((n-k-1)/2)} ( sign(floor((i+k)/(n-i-k+1))) *
 *            mu(i)^2 * mu(k)^2 * mu(n-i-k)^2 )
 * Range:     u8 n <= 38, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* loop over smallest side k */
    for (idx_t k = 1; k <= (n - 1) / 3; ++k)
    {
        /* squarefree test for k */
        if ((long long)mobius(k) == 0)
            continue;

        /* loop over middle side i */
        idx_t i_max = (n - k - 1) / 2;
        for (idx_t i = k + 1; i <= i_max; ++i)
        {
            /* squarefree test for i */
            if ((long long)mobius(i) == 0)
                continue;

            idx_t c = n - k - i;           /* largest side */
            /* squarefree test for c */
            if ((long long)mobius(c) == 0)
                continue;

            /* triangle inequality k + i > c is equivalent to sign > 0 */
            if (k + i > c)
                sum += 1;
        }
    }

    return (seq_t)n * sum;
}
