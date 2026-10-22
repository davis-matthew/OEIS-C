/*
 * A322209  L.g.f.: log( Product_{n>=1} 1/(1 - (2^n+1)*x^n) ).
 * https://oeis.org/A322209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} d * (2^d + 1)^(n/d) * (with a(0) = 0) * * The
 *            formula follows from the logarithmic generating function * L(x)
 *            = log(∏_{m≥1} 1/(1-(2^m+1)x^m)). The coefficient of x^n in *
 *            L(x) equals n⁻¹ * a(n), which gives the above divisor sum
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/pow_seq.h"

/* Return the n-th term of A322209.  n is 0‑based. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* a(0) is defined to be 0 */

    seq_t sum = 0;
    idx_t d, other;

    for (d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* process divisor d */
        {
            seq_t pow2_d = big_pow((seq_t)2, (seq_t)d);   /* 2^d   */
            seq_t base  = pow2_d + (seq_t)1;             /* 2^d+1 */
            int    exp  = (int)(n / d);                /* n/d   */
            seq_t term  = pow_seq(base, exp);           /* base^exp */
            term        = term * (seq_t)d;              /* d * base^exp */
            sum        += term;                         /* accumulate */
        }

        other = n / d;
        if (other == d)
            continue;     /* avoid double‑counting the square divisor */

        /* process the complementary divisor other */
        {
            seq_t pow2_other = big_pow((seq_t)2, (seq_t)other);
            seq_t base       = pow2_other + (seq_t)1;
            int    exp       = (int)(n / other); /* which equals d */
            seq_t term       = pow_seq(base, exp);
            term             = term * (seq_t)other;
            sum              += term;
        }
    }

    return sum;
}
