/*
 * A391889  a(n) = (7*n)!/(n!*(2*n)!*(4*n)!).
 * https://oeis.org/A391889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(7*n,3*n) * binomial(3*n,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/*
 * Computes the nth term of OEIS A391889:
 *   a(n) = (7*n)!/(n!*(2*n)!*(4*n)!)
 *
 * For all supported integer types of seq_t and idx_t.
 * The implementation uses the provided binomial function for the
 * two binomial coefficients and multiplies the results with the
 * built-in multiplication operator.
 */
seq_t a(idx_t n)
{
    /* Compute intermediates 7*n and 3*n */
    idx_t seven_n  = n * (idx_t)7;
    idx_t three_n  = n * (idx_t)3;

    /* Binomial coefficients */
    seq_t B1 = binomial(seven_n, three_n);   /* binomial(7n,3n) */
    seq_t B2 = binomial(three_n, n);         /* binomial(3n,n) */

    /* Product gives the desired term */
    return B1 * B2;
}
