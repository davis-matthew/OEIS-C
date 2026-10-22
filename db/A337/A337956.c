/*
 * A337956  Number of oriented colorings of the 8 cubic facets of a tesseract
 *          or of the 8 vertices of a hyperoctahedron.
 * https://oeis.org/A337956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(binomial(n+1,2)+3, 4) +
 *            binomial(binomial(n,2),4)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is not defined; return 0 for safety. */
    if (n == 0)
        return 0;

    /* Compute the two inner binomial arguments. */
    idx_t t1 = n * (n + 1) / 2 + 3;   /* C(n+1,2)+3 */
    idx_t t2 = n * (n - 1) / 2;       /* C(n,2)   */

    /* Evaluate the outer binomial coefficients. */
    seq_t v1 = binom(t1, 4);          /* C(t1,4)  */
    seq_t v2 = binom(t2, 4);          /* C(t2,4)  */

    return v1 + v2;
}
