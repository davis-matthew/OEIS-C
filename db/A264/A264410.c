/*
 * A264410  G.f.: Sum_{n>=0} (n^2)!/n!^(n+1) * x^n / (1-x)^(n^2+1).
 * https://oeis.org/A264410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (1/k!) * Product_{j=0..k-1} binomial(n +
 *            j*k, k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t fac_k = 1;                      /* factorial(k) for current k */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t prod = 1;
        for (idx_t j = 0; j < k; ++j) {
            prod *= binomial(n + j * k, k);
        }
        sum += prod / fac_k;
        fac_k *= (k + 1);                /* prepare factorial(k+1) for next loop */
    }

    return sum;
}
