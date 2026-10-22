/*
 * A387967  E.g.f. A(x) satisfies A(x) = exp( x*A(x) * (1+x*A(x))^3 ) *
 *          (1+x*A(x)).
 * https://oeis.org/A387967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n+1)^(k-1) * binomial(n+3k+1,n-k)/k!
 *            (OEIS A387967)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Multiply *val by mul and divide by div, keeping the result integral.
   The division is exact for the intended use of this routine.               */
static inline void mul_div(seq_t *val, seq_t mul, seq_t div)
{
    /* First cancel common factors between *val and the divisor */
    seq_t g1 = gcd(*val, div);
    if (g1 > 1) {
        *val /= g1;
        div /= g1;
    }
    /* Then cancel factors between the multiplier and the remaining divisor */
    seq_t g2 = gcd(mul, div);
    if (g2 > 1) {
        mul /= g2;
        div /= g2;
    }
    /* The divisor should now be 1 (exact division) */
    *val = (*val) * mul;
}

/* Compute the n-th term of OEIS A387967 */
seq_t a(idx_t n)
{
    /* term = n!  (k = 0 term) */
    seq_t term = 1;
    for (idx_t i = 1; i <= n; ++i) {
        term *= (seq_t)i;          /* factorial */
    }

    seq_t sum = term;              /* sum of terms found so far */

    /* Update term for k = 1 .. n and add to sum */
    for (idx_t k = 0; k < n; ++k) {
        /* ratio from term_k to term_{k+1} :
           term_{k+1} = term_k * (n+1)/(k+1) *
                         (N+1)(N+2)(N+3)*r / ((N-r+1)(N-r+2)(N-r+3)(N-r+4))
           where N = n+3k+1, r = n-k
        */
        mul_div(&term, (seq_t)(n + 1), (seq_t)(k + 1));

        seq_t N = (seq_t)(n + 3 * k + 1);
        seq_t r = (seq_t)(n - k);

        mul_div(&term, N + 1, N - r + 1);
        mul_div(&term, N + 2, N - r + 2);
        mul_div(&term, N + 3, N - r + 3);
        mul_div(&term, r, N - r + 4);

        sum += term;
    }

    return sum;
}
