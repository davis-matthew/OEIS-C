/*
 * A278704  Number of triangles in all simple labeled graphs on n nodes.
 * https://oeis.org/A278704
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,3) * 2^(binomial(n,2)-3)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;                       /* undefined below offset */

    /* C(n,3) and C(n,2) are computed using the provided helper. */
    seq_t triangles = binom(n, 3);
    seq_t edges     = binom(n, 2);

    /* exponent for 2 is C(n,2)-3, cast to idx_t for pow2. */
    idx_t exp = (idx_t)(edges - (seq_t)3);

    seq_t two_pow = pow2(exp);
    return triangles * two_pow;
}
