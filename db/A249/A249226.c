/*
 * A249226  Denominators of constants A(a) related to the asymptotic LCM of
 *          arithmetic progressions a*n+b (a and b coprime).
 * https://oeis.org/A249226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (n/phi(n)) * Sum_{gcd(j,n)=1} 1/j
 *            (A249226: denominators of constants A(n))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 24, u64 n <= 46, u128 n <= 88,
 *            big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"   /* gcd(seq_t,a,seq_t,b) */
#include "../utils/phi.h"   /* seq_t phi(idx_t n) */

static void add_one_over_j(seq_t *num, seq_t *den, idx_t j)
{
    seq_t jj = (seq_t)j;
    seq_t g   = gcd(*den, jj);
    seq_t lcm = (*den / g) * jj;
    seq_t f1  = jj / g;          /* factor for existing numerator   */
    seq_t f2  = *den / g;        /* factor for 1/j                   */
    seq_t new_num = (*num) * f1 + f2;
    seq_t new_den = lcm;
    seq_t d = gcd(new_num, new_den);
    *num = new_num / d;
    *den = new_den / d;
}

seq_t a(idx_t n)
{
    /* Compute S = Sum_{j=1..n, gcd(j,n)=1} 1/j   as reduced fraction */
    seq_t num = 0;   /* numerator   */
    seq_t den = 1;   /* denominator */

    for (idx_t j = 1; j <= n; ++j)
    {
        if (gcd((seq_t)j, (seq_t)n) == 1)
            add_one_over_j(&num, &den, j);
    }

    /* Multiply by n/phi(n) and reduce */
    seq_t phi_n   = phi(n);                    /* φ(n) */
    seq_t a_num   = num * (seq_t)n;            /* numerator of A(n) */
    seq_t a_den   = den * phi_n;               /* denominator of A(n) */

    seq_t g = gcd(a_num, a_den);
    a_num /= g;
    a_den /= g;

    return a_den;      /* denominator of the reduced fraction */
}
