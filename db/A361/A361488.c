/*
 * A361488  Diagonal of rational function 1/(1 - (x^3 + y^3 + x^4*y)).
 * https://oeis.org/A361488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Sum_{k=0..⌊n/3⌋} binomial(2*k, k) * binomial(k, n-3*k)
 *            * (OEIS A361488 – diagonal of 1/(1-(x^3+y^3+x^4*y))
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 41, u64 n <= 78, u128 n <= 152,
 *            big any n
 * Helpers:   binomial, prod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

/* Use the utility binomial if available */
#ifndef binomial
#include "../utils/binomial.h"
#include "../utils/prod.h"
#endif

seq_t a(idx_t n)
{
    /* Prefix sums for binomial coefficients may be reused if called
     * many times, but the linear time of the loop up to n/3 is
     * sufficiently fast for the required limits. */
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t c1 = binomial(2 * k, k);          /* C(2k,k) */
        idx_t r = n - 3 * k;
        seq_t c2 = (r > k) ? 0 : binomial(k, r);  /* C(k, r) */
        if (c1 == 0 || c2 == 0)
            continue;
        sum += prod(c1, c2);
    }
    return sum;
}
