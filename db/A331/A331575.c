/*
 * A331575  a(n) is the number of subsets of {1..n} that contain 4 even and 4
 *          odd numbers.
 * https://oeis.org/A331575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n/2,4)^2 for even n, a(n) = binomial((n-1)/2,4)
 *            * binomial((n+1)/2,4) for odd n
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 73, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the provided binomial helper */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For both even and odd n we need to consider
       floor(n/2) and ceil(n/2) which are n/2 and n/2+1 for odd n */
    idx_t half = n / 2;
    seq_t c1 = binomial(half, 4);
    if ((n & 1u) == 0)          /* n is even */
    {
        return c1 * c1;
    }
    else                        /* n is odd */
    {
        seq_t c2 = binomial(half + 1, 4);
        return c1 * c2;
    }
}
