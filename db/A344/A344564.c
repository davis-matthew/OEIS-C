/*
 * A344564  a(n) = [x^n] -3/(2*x - 1)^5.
 * https://oeis.org/A344564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n+4)*a(n-1)/n, a(0) = 3, a(1) = 30
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 17, u64 n <= 44, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __SEQ_LIMITS_H
#include <stdint.h>  /* for uint64_t, etc. */
#endif

/* Compute gcd for any unsigned integral type used as seq_t */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)30;

    seq_t cur = (seq_t)30;            /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* Reduce the division by n from a(i-1) to avoid overflow */
        seq_t g1 = seq_gcd(cur, (seq_t)i);
        seq_t a_div = cur / g1;
        seq_t denom  = (seq_t)i / g1;

        /* Compute the factor 2*(i+4) */
        seq_t factor = (seq_t)2 * ((seq_t)i + (seq_t)4);

        /* Further reduce any common part with the remaining denominator */
        seq_t g2 = seq_gcd(factor, denom);
        factor /= g2;
        denom  /= g2;

        /* After these reductions denom should be 1 (integer result) */
        cur = a_div * factor;          /* safe: product fits if result fits */
    }
    return cur;
}
