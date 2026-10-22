/*
 * A362694  E.g.f. satisfies A(x) = exp(x + x * A(x)^2).
 * https://oeis.org/A362694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (2*k+1)^(n-1) * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t result = 0;
    seq_t exp = (seq_t)(n - 1);          /* exponent is n-1 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* (2*k+1) fits in seq_t for n where answer fits */
        seq_t base = (seq_t)(2 * k + 1);

        /* compute (2*k+1)^(n-1) */
        seq_t pow_val = big_pow(base, exp);

        /* binomial coefficient C(n,k) */
        seq_t comb = binomial(n, k);

        /* accumulate */
        result += pow_val * comb;
    }

    return result;
}
