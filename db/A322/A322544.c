/*
 * A322544  a(n) is the reciprocal of the coefficient of x^n in the power
 *          series defined by ((1+2x)*exp(x) + 3*exp(-x) - 4)/ (4x^2).
 * https://oeis.org/A322544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*(n+2)!)/(2n+5+3*(-1)^n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t
gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Build the denominator d = 2*n + 5 + 3*(-1)^n */
    seq_t denom = (seq_t)2 * (seq_t)n + (seq_t)5;
    if ((n & 1ULL) == 0)          /* n even -> add 3  */
        denom += (seq_t)3;
    else                          /* n odd  -> subtract 3 */
        denom -= (seq_t)3;

    /* Cancel factors of denom with the factorial (n+2)! */
    seq_t res = 1;
    for (idx_t i = 1; i <= n + 2; ++i) {
        seq_t term = (seq_t)i;
        seq_t g = gcd_seq(term, denom);
        if (g > 1) {
            term   /= g;
            denom  /= g;
        }
        res *= term;
    }

    /* Multiply by the remaining factor 4, canceling with denom if needed */
    seq_t g = gcd_seq((seq_t)4, denom);
    if (g > 1) {
        denom /= g;
        res   *= (seq_t)(4 / g);
    } else {
        res   *= (seq_t)4;
    }

    /* If any part of the denominator remains, divide it out.  In theory denom
       should now be 1 for all n >= 0, but this handles any unforeseen cases. */
    if (denom > 1)
        res /= denom;

    return res;
}
