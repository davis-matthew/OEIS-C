/*
 * A365016  E.g.f. satisfies A(x) = exp( x*A(x)^3/(1 - x * A(x)^2) ).
 * https://oeis.org/A365016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} (2*n + k + 1)^(k-1) * binomial(n-1,
 *            k-1) / k! (a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) -> seq_t */
#include "../utils/pow_seq.h"    /* pow_seq(seq_t base, idx_t exp) -> seq_t */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* cur holds n! / k! for the current k.  Start with k = 1:
       cur = n! / 1! = n! / 1 = product_{i=2}^{n} i                      */
    seq_t cur = 1;
    for (idx_t i = n; i >= 2; --i)
        cur *= (seq_t)i;

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* base = 2*n + k + 1, exponent = k-1 */
        seq_t base = (seq_t)(2 * n + k + 1);
        seq_t pow_term = pow_seq(base, (idx_t)(k - 1));

        /* binomial(n-1, k-1) */
        seq_t binom = binomial(n - 1, k - 1);

        /* term = (n! / k!) * binom * base^(k-1) */
        seq_t term = cur * binom * pow_term;

        sum += term;

        /* Prepare cur for the next k (i.e., n! / (k+1)!):
           cur_new = cur / (k+1)  (for k < n only) */
        if (k < n)
            cur = cur / (seq_t)(k + 1);
    }

    return sum;
}
