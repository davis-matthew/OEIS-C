/*
 * A275655  a(n) = binomial(6*n,3*n)*binomial(2*n,n).
 * https://oeis.org/A275655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(6*n,3*n) * binomial(2*n,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided binomial function. */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the two binomial coefficients separately. */
    idx_t n6 = n * 6;
    idx_t n3 = n * 3;
    idx_t n2 = n * 2;

    seq_t c1 = binomial(n6, n3);   /* C(6n,3n) */
    seq_t c2 = binomial(n2, n);    /* C(2n,n) */

    /* Multiply the results. */
    return c1 * c2;
}
