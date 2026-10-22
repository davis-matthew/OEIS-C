/*
 * A340677  a(n) = A007947(n) / gcd(A007947(n), A008472(n)).
 * https://oeis.org/A340677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007947(n) / gcd(A007947(n), A008472(n))
 * Range:     u8 n <= 257, u16 n <= 9986, u32 n <= 9986, u64 n <= 9986,
 *            u128 n <= 9986, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t rad = (seq_t)1;
    seq_t sum = (seq_t)0;

    idx_t temp = n;
    /* trial division to find distinct prime factors */
    for (idx_t d = 2; d <= temp / d; ++d)
    {
        if (temp % d == 0)
        {
            rad *= (seq_t)d;
            sum += (seq_t)d;
            while (temp % d == 0)
                temp /= d;
        }
    }
    if (temp > 1)
    {
        rad *= (seq_t)temp;
        sum += (seq_t)temp;
    }

    seq_t g = gcd(rad, sum);
    return rad / g;
}
