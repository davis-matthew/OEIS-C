/*
 * A354436  a(n) = n! * Sum_{k=0..n} k^(n-k)/k!.
 * https://oeis.org/A354436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n} (n-k)^k * k! * binomial(n,k)
 *
 * This function evaluates the summation directly.  It uses the
 * multiplicative recurrence for binomial coefficients and factorial
 * values to avoid large intermediate factorials.  Powers are computed
 * with the helper pow_seq which performs exponentiation by squaring.
 */
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* The sequence starts with a(0) = 1 */

    seq_t sum = 0;                  /* Accumulator for the sum */
    seq_t binom = 1;                /* binomial(n, 0) */
    seq_t fact = 1;                 /* fact(0)         */

    for (idx_t k = 0; k <= n; ++k) {
        /* (n-k)^k */
        seq_t pow_term = pow_seq((seq_t)(n - k), (int)k);

        /* term = binom * fact * pow_term */
        seq_t term = binom * fact;
        term *= pow_term;

        sum += term;

        /* Prepare binom and fact for k+1 */
        if (k < n) {
            /* binom(n, k+1) = binom(n, k) * (n-k) / (k+1) */
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
            /* fact(k+1) = fact(k) * (k+1) */
            fact = fact * (seq_t)(k + 1);
        }
    }

    return sum;
}
