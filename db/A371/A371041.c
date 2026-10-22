/*
 * A371041  E.g.f. satisfies A(x) = exp(x^2*A(x)^3) / (1-x).
 * https://oeis.org/A371041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/2⌋} (3k+1)^(k-1) * binomial(n+k, n-2k) /
 *            k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, factorial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t, if needed */
#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    /* factorial(n) */
    seq_t fact_n = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact_n *= (seq_t)i;

    /* iteratively build factorial(k) */
    seq_t fact_k = 1;   /* k = 0 */
    seq_t result = 0;

    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term = fact_n;
        term /= fact_k;                     /* divide by k! */

        if (k > 0) {
            seq_t base = 3 * k + 1;
            term *= pow_seq(base, (int)(k - 1)); /* (3k+1)^(k-1) */
        }

        /* binomial(n+k, n-2k) */
        seq_t bin = binomial(n + k, n - 2 * k);
        term *= bin;

        result += term;

        /* prepare factorial(k+1) for next iteration */
        if (k < maxk)
            fact_k *= (seq_t)(k + 1);
    }

    return result;
}
