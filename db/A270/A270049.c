/*
 * A270049  Number of 123 avoiding set partitions of [n].
 * https://oeis.org/A270049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n/2} binomial(n,2*i)*(2*i)!!
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 30, u128 n <= 53,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum  = 1;   /* term for i = 0 is binomial(n,0)*(0)!! = 1  */
    seq_t term = 1;   /* term_i  = binomial(n,2*i)*(2*i)!!            */

    idx_t max_i = n / 2;
    for (idx_t i = 1; i <= max_i; ++i)
    {
        idx_t k = i - 1;                   /* previous i value          */

        seq_t denom = (seq_t)(2 * k + 1);   /* (2k+1) part of the denominator */

        /* cancel factors between term and denom */
        seq_t g = gcd(term, denom);
        term   = term / g;
        denom  = denom / g;

        /* n - 2k  factor */
        seq_t a_part = (seq_t)(n - 2 * k);
        g = gcd(a_part, denom);
        a_part = a_part / g;
        denom  = denom / g;

        /* n - 2k - 1  factor */
        seq_t b_part = (seq_t)(n - 2 * k - 1);
        g = gcd(b_part, denom);
        b_part = b_part / g;
        denom  = denom / g;

        /* denom should now be 1; multiply the remaining factors */
        term = term * a_part;
        term = term * b_part;

        sum += term;
    }

    return sum;
}
