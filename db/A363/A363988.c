/*
 * A363988  a(n) = Sum_{k = 0..n}
 *          (-4)^(n-k)*binomial(n,k)*binomial(3*n+2*k,2*k)* binomial(2*k,k).
 * https://oeis.org/A363988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (-4)^(n-k) * binomial(n,k) *
 *            binomial(3*n+2*k,2*k) * binomial(2*k,k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t k = 0; k <= n; ++k) {
        idx_t exp = n - k;                /* exponent of (-4) */
        seq_t pow = pow_uint(4, exp);    /* absolute value of (-4)^(n-k) */

        /* compute the current term */
        seq_t term = pow;
        term = term * binomial(n, k);
        term = term * binomial(3 * n + 2 * k, 2 * k);
        term = term * binomial(2 * k, k);

        /* add term to the appropriate accumulator */
        if ((exp & 1ULL) == 0ULL) {
            pos += term;                 /* even exponent → positive */
        } else {
            neg += term;                 /* odd exponent  → negative */
        }
    }

    /* all sequence values are known to be non‑negative,
       hence pos >= neg and subtraction is safe for the unsigned type */
    return pos - neg;
}
