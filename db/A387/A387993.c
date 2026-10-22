/*
 * A387993  E.g.f. A(x) satisfies A(x) = exp( x^2*A(x)^2 * (1+x*A(x))^3 ).
 * https://oeis.org/A387993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (n+1)^(k-1) *
 *            binomial(3*k,n-2*k)/k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    idx_t half = n / 2;
    seq_t n_fact = factorial(n);     /* n! */
    seq_t sum = 0;
    seq_t pow_factor = 1;            /* (n+1)^(k-1) for k=1 starts at 1 */
    seq_t k_fact = 1;                /* k! */
    seq_t n_plus1 = (seq_t)(n + 1);

    for (idx_t k = 1; k <= half; ++k) {
        k_fact *= (seq_t)k;                     /* update k! */
        seq_t bin = binomial(3 * k, n - 2 * k); /* C(3k, n-2k) */
        seq_t nfk = n_fact / k_fact;            /* n! / k! */
        seq_t term = nfk * bin;                 /* (n!/k!)*C(3k,n-2k) */
        term *= pow_factor;                     /* multiply by (n+1)^(k-1) */
        sum += term;                            /* add to total */
        pow_factor *= n_plus1;                  /* prepare for next k */
    }
    return sum;
}
