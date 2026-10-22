/*
 * A375172  Expansion of e.g.f. exp( x^2/(1-x)^3 ) / (1-x).
 * https://oeis.org/A375172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/2⌋} binomial(n+k, n-2*k)/k!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute n! once. */
    seq_t fac_n = factorial(n);

    seq_t sum = 0;
    seq_t R   = fac_n;          /* R = n! / k!  (initial k = 0) */

    idx_t max_k = n / 2;        /* floor(n/2) */

    for (idx_t k = 0; k <= max_k; ++k) {
        if (k > 0) {
            /* R = (n!)/(k!)  = (n!)/( (k-1)! * k )  = R/(k)  */
            R /= k;
        }

        /* binomial(n+k, n-2*k) is guaranteed to be an integer */
        seq_t bn   = binomial(n + k, n - 2 * k);
        seq_t term = R * bn;     /* term = (n! / k!) * binomial(...) */
        sum += term;
    }

    return sum;
}
