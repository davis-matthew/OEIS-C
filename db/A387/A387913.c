/*
 * A387913  Hodge number h^{1,n-1} of a smooth hypersurface of degree n+2 in
 *          P^{n+1}.
 * https://oeis.org/A387913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+3, n+2) - (n+2)^2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute k = n + 2 for the binomial coefficient */
    idx_t k = n + 2;

    /* binomial(2*n+3, k) returns a seq_t value */
    seq_t bin  = binomial(2 * n + 3, k);

    /* (n+2)^2 computed in seq_t arithmetic */
    seq_t t = (seq_t)(n + 2);
    seq_t sq = t * t;

    return bin - sq;
}
