/*
 * A371040  E.g.f. satisfies A(x) = exp(x^3*A(x)^2) / (1-x).
 * https://oeis.org/A371040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, factorial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A371040
   Formula: a(n) = Σ_{k=0..⌊n/3⌋} (n! / k!) * (2k+1)^(k-1) * binomial(n-k, n-3k)
   (a(0)=1) */

#include "../utils/binomial.h"   /* binomial(idx_t, idx_t) */
#include "../utils/pow_int.h"    /* pow_int(seq_t, idx_t) */
#include "../utils/factorial.h"  /* factorial(idx_t) */

seq_t a(idx_t n)
{
    seq_t fact_n = factorial(n);          /* n!  (exact integer) */
    seq_t sum    = 0;                     /* running total */
    seq_t fact_k = 1;                     /* k!  initially 0! = 1 */

    idx_t max_k = n / 3;                  /* floor(n/3) */

    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t bin = binomial(n - k, n - 3 * k);

        seq_t pow_val;
        if (k == 0) {
            pow_val = 1;                  /* (2*0+1)^{-1} = 1 */
        } else {
            pow_val = pow_int((seq_t)(2 * k + 1), k - 1);
        }

        seq_t term = fact_n / fact_k;     /* n! / k!  (integer) */
        term *= pow_val;                 /* * (2k+1)^(k-1) */
        term *= bin;                     /* * binomial(...) */

        sum += term;                     /* accumulate integer sum */

        /* prepare k! for next iteration */
        if (k < max_k) {
            fact_k *= (k + 1);
        }
    }

    return sum;                          /* final sequence value */
}
