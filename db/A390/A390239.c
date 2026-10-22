/*
 * A390239  a(n) = Sum_{k=0..n} binomial(3*n-2*k+2,n-k).
 * https://oeis.org/A390239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+2*k+2, k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive update of binomial terms using integer‐preserving cancellations. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t C   = 1;          /* C_k = binomial(n+2*k+2, k), starting with k=0 */

    for (idx_t k = 0; k <= n; ++k)
    {
        sum += C;
        if (k == n)
            break;

        /* Update C to C_{k+1}:
           C_{k+1} = C_k * (n+2k+4)(n+2k+3) / ((k+1)(n+k+3))   */

        /* convert arithmetic to seq_t to avoid overflow of intermediate idx_t */
        seq_t a_ = (seq_t)n + 2*(seq_t)k + 4;   /* numerator factor 1 */
        seq_t b_ = (seq_t)n + 2*(seq_t)k + 3;   /* numerator factor 2 */
        seq_t c_ = (seq_t)(k + 1);              /* denominator factor 1 */
        seq_t d_ = (seq_t)n + (seq_t)k + 3;      /* denominator factor 2 */

        /* cancel common factors between numerators and denominators */
        seq_t g = gcd(a_, c_);
        a_ /= g;   c_ /= g;
        g = gcd(b_, d_);
        b_ /= g;   d_ /= g;
        g = gcd(a_, d_);
        a_ /= g;   d_ /= g;
        g = gcd(b_, c_);
        b_ /= g;   c_ /= g;

        /* cancel any remaining factors from the current binomial value */
        g = gcd(C, c_);
        C /= g;   c_ /= g;
        g = gcd(C, d_);
        C /= g;   d_ /= g;

        /* perform the multiplication and final division */
        C = C * a_ * b_;
        if (c_ > 1) C /= c_;
        if (d_ > 1) C /= d_;
    }

    return sum;
}
