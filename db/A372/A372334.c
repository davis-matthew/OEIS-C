/*
 * A372334  Expansion of e.g.f. -exp(x) * LambertW(-3*x)/3.
 * https://oeis.org/A372334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (3*k)^(k-1) * binomial(n, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

/* Compute A372334: the expansion of e.g.f. -exp(x) * LambertW(-3*x)/3. */
seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0)
        return 0;

    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* (3*k)^(k-1) */
        seq_t pow_term = big_pow((seq_t)(3 * k), k - 1);
        /* binomial coefficient C(n, k) */
        seq_t bin_coef = binomial(n, k);
        /* add the term to the running sum */
        total += pow_term * bin_coef;
    }
    return total;
}
