/*
 * A372928  a(n) = Sum_{1 <= x_1, x_2, x_3 <= n} gcd(x_1, x_2, x_3, n)^3.
 * https://oeis.org/A372928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * d^3 * tau(d)
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 599, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation based on the multiplicative closed form
   a(p^e) = p^(3e-3) * (p^3*(e+1) - e)  for every prime power in n. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* defined only for n >= 1 (offset 1) */

    uint64_t pr[64], ex[64];
    size_t nf = 0;

    /* factorization of n */
    factor_u64((uint64_t)n, pr, ex, &nf);

    seq_t result = 1;

    for (size_t i = 0; i < nf; ++i) {
        seq_t p = (seq_t)pr[i];
        uint64_t e = ex[i];

        /* compute p^(3e-3)  (for e >= 1 always) */
        idx_t exp_pow = 3 * e - 3;           /* 3*e-3 fits in idx_t for all e <= 64 */
        seq_t p_pow = seq_pow(p, exp_pow);

        /* compute p^3 */
        seq_t p3 = seq_pow(p, 3);

        /* term inside the bracket: p^3*(e+1) - e */
        seq_t term_inside = p3 * (seq_t)(e + 1) - (seq_t)e;

        result *= p_pow * term_inside;
    }

    return result;
}
