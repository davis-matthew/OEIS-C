/*
 * A380755  E.g.f. A(x) satisfies A(x) = exp(x * A(x)^2 / (1 - x * A(x)^2)^2)
 *          / (1 - x * A(x)^2).
 * https://oeis.org/A380755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (2*n+1)^(k-1) * binomial(3*n+k, n-k)/k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* outer factorial factor */
    seq_t fact = factorial(n);

    /* common factor 2*n+1 */
    seq_t two_n_plus1 = (seq_t)(2 * n + 1);

    /* k = 0 term: n! * binomial(3n, n) / (2*n+1) */
    seq_t bin0 = binomial(3 * n, n);
    seq_t term0 = fact * bin0;
    term0 = term0 / two_n_plus1;          /* exact division */
    seq_t sum = term0;

    /* prepare for k >= 1 terms */
    seq_t mult = fact;        /* will hold n! / k! */
    seq_t pow  = 1;           /* (2*n+1)^(k-1), starting with k=1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* update n! / k! */
        mult = mult / k;                     /* exact division */
        /* binomial(3n+k, n-k) */
        seq_t bin = binomial(3 * n + k, n - k);
        /* term = (n! / k!) * bin * (2*n+1)^(k-1) */
        seq_t term = mult * bin;
        term = term * pow;
        sum += term;
        /* advance power for next k */
        pow = pow * two_n_plus1;
    }

    return sum;
}
