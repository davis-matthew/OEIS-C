/*
 * A352373  a(n) = [x^n] ( 1/((1 - x)^2*(1 - x^2)) )^n for n >= 1.
 * https://oeis.org/A352373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*n-2*k-1, n-2*k) *
 *            binomial(n+k-1, k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 835 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the binom helper from the utilities. */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n < 1, return 0 */

    seq_t result = 0;
    idx_t half = n / 2;          /* floor(n/2) */

    for (idx_t k = 0; k <= half; ++k)
    {
        seq_t term = binomial(3*n - 2*k - 1, n - 2*k);
        term *= binomial(n + k - 1, k);
        result += term;
    }
    return result;
}
