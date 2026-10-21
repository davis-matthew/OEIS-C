/*
 * A294982  Number of compositions (ordered partitions) of 1 into exactly 3n+1
 *          powers of 1/(n+1).
 * https://oeis.org/A294982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n+1,n+1)*binomial(2*n,n)+binomial(3*n+1,n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t c1 = binomial(3*n + (idx_t)1, n + (idx_t)1);
    seq_t c2 = binomial(2*n, n);
    seq_t term1 = c1 * c2;
    seq_t term2 = binomial(3*n + (idx_t)1, n - (idx_t)1);

    return term1 + term2;
}
