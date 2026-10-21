/*
 * A295436  a(n) = (12*n)!*(5*n)!/((10*n)!*(4*n)!*(3*n)!).
 * https://oeis.org/A295436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (12*n)!*(5*n)! / ((10*n)!*(4*n)!*(3*n)!) Equivalent: a(n)
 *            = binomial(12*n,2*n) * binomial(5*n,n) / binomial(3*n,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute needed binomial coefficients. */
    seq_t bin12_2 = binomial(12 * n, 2 * n);   /* (12n choose 2n)   */
    seq_t bin5_1  = binomial(5 * n, n);        /* (5n  choose n )   */
    seq_t bin3_1  = binomial(3 * n, n);        /* (3n  choose n )   */

    /* Cancel gcd between bin5_1 and bin3_1. */
    seq_t g = gcd(bin5_1, bin3_1);
    bin5_1 /= g;
    bin3_1 /= g;

    /* Cancel gcd between bin12_2 and remaining bin3_1. */
    g = gcd(bin12_2, bin3_1);
    bin12_2 /= g;
    bin3_1 /= g;

    /* At this point bin3_1 should be 1.  If not, it divides the final product exactly. */
    /* Multiply the remaining factors. */
    return bin12_2 * bin5_1;
}
