/*
 * A378093  E.g.f. satisfies A(x) = exp( x * (1-x)^2 * A(x)^3 ) / (1-x).
 * https://oeis.org/A378093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..n} (3k+1)^(k-1) * binomial(n,k) / k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation evaluates a(n) directly using the summation form:
   a(n) = Σ_{k=0..n} (3k+1)^(k-1) * binomial(n,k) * Π_{t=k+1..n} t
   where Π over an empty set equals 1.  The algorithm walks k from n down to 0,
   maintaining the binomial value and the product Π_{t=k+1..n} t without any
   divisions (all intermediate divisions are exact).  The big‑integer
   operations rely on the utility functions provided in the build framework. */

#include "../utils/pow_int.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t prod = 1;          /* Π_{t=k+1..n} t, starts empty for k = n */
    seq_t binom = 1;         /* binomial(n,n) = 1 */

    for (idx_t k = n; ; --k)
    {
        seq_t base = (seq_t)(3 * k + 1);
        idx_t  exp  = (k == 0) ? 0 : (k - 1);
        seq_t  pow_val = pow_int(base, exp);               /* (3k+1)^(k-1) */

        seq_t term = pow_val * binom * prod;
        sum += term;

        if (k == 0)
            break;    /* finished */

        /* Update prod for the next k-1: multiply by k */
        prod *= (seq_t)k;

        /* Update binom from binomial(n,k) to binomial(n,k-1) */
        binom = binom * (seq_t)k / (seq_t)(n - k + 1);
    }

    return sum;
}
