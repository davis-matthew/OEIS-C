/*
 * A284268  Sum of coefficients > 1 in the Stern polynomial B(2n+1,x): a(n) =
 *          A275812(A277324(n)).
 * https://oeis.org/A284268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of coefficients > 1 in Stern polynomial B(2n+1, x)
 * Range:     u8 n <= 2344, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   polynomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/polynomial.h"

/* Primary function computing a(n). */
seq_t a(idx_t n)
{
    idx_t m = 2 * n + 1;                /* odd index of the Stern polynomial */
    size_t len;
    seq_t *coeffs = polynomial(m, &len);

    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        if (coeffs[i] > 1)
            sum += coeffs[i];
    }

    free(coeffs);
    return sum;
}
