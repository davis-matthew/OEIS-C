/*
 * A277614  a(n) is the coefficient of x^n/n! in exp(x + n*x^2/2).
 * https://oeis.org/A277614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n,2k) * (2k-1)!! * n^k
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t result = 1;    /* term for k = 0 */
    seq_t term   = 1;    /* current term_k */

    idx_t max_k = n / 2;
    seq_t n_seq = (seq_t)n;

    for (idx_t k = 0; k < max_k; ++k)
    {
        /* Compute term_{k+1} from term_k:
           term_{k+1} = term_k * n * (n-2k) * (n-2k-1) / (2k+2) */
        seq_t denom = (seq_t)(2 * k + 2);
        seq_t num1  = n_seq;
        seq_t num2  = (seq_t)(n - 2 * k);
        seq_t num3  = (seq_t)(n - 2 * k - 1);

        /* Reduce numerator factors with denominator to keep intermediates small. */
        seq_t g;
        g = gcd(num1, denom); num1 /= g; denom /= g;
        g = gcd(num2, denom); num2 /= g; denom /= g;
        g = gcd(num3, denom); num3 /= g; denom /= g;

        term = term * num1;
        term = term * num2;
        term = term * num3;
        if (denom != 1) term /= denom;

        result += term;
    }

    return result;
}
