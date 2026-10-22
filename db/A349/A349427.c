/*
 * A349427  a(n) = ((n+1)^2 - 2) * binomial(2*n-2,n-1) / 2.
 * https://oeis.org/A349427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)^2 - 2) * binomial(2*n-2, n-1) / 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0)
        return 0;

    /* binomial(2*n-2, n-1)      */
    seq_t bin = binomial(2 * n - 2, n - 1);

    /* ((n+1)^2 - 2)              */
    seq_t np1   = (seq_t)(n + 1);
    seq_t factor = np1 * np1 - 2;

    /* Avoid overflow by dividing the even factor first */
    seq_t result;
    if ((bin & 1) == 0) {            /* bin is even – divide it */
        seq_t half  = bin / 2;
        result = half * factor;
    } else {                         /* bin is odd – factor must be even */
        seq_t half  = factor / 2;
        result = bin * half;
    }

    return result;
}
