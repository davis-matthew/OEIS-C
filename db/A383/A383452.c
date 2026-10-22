/*
 * A383452  Column 3 in A104978.
 * https://oeis.org/A383452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3 + 2*n)! / (6*(n - 3)!*(n + 4)!) for n >= 3
 *            (a(0)=a(1)=a(2)=0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    /* binomial coefficient binom(2*n+3, n-3) */
    idx_t upper = 2 * n + 3;
    idx_t lower = n - 3;
    seq_t bin = binomial(upper, lower);

    /* Reduce the factor 6 among bin, n+5, n+6 to avoid overflow */
    seq_t factor = (seq_t)6;

    seq_t g = gcd(bin, factor);
    bin /= g;
    factor /= g;

    seq_t n5 = (seq_t)(n + 5);
    g = gcd(n5, factor);
    n5 /= g;
    factor /= g;

    seq_t n6 = (seq_t)(n + 6);
    g = gcd(n6, factor);
    n6 /= g;
    factor /= g;

    /* After reductions factor should be 1 */
    seq_t result = bin;
    result *= n5;
    result *= n6;

    return result;
}
