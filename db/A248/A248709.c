/*
 * A248709  a(n) = f(5*n)/(f(n-2)*f(n-1)*f(n)*f(n+1)*f(n+2)), where f(k) = k!.
 * https://oeis.org/A248709
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = f(5*n)/(f(n-2)*f(n-1)*f(n)*f(n+1)*f(n+2)) where f(k)=k!
 * Range:     u8 none, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 199 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2.  For n < 2 we return 0. */
    if (n < 2u)
        return (seq_t)0;

    seq_t res = (seq_t)1;
    /* Multinomial coefficient as a product of binomials. */
    res *= binomial(5 * n, n - 2);
    res *= binomial(4 * n + 2, n - 1);
    res *= binomial(3 * n + 3, n);
    res *= binomial(2 * n + 3, n + 1);

    return res;
}
