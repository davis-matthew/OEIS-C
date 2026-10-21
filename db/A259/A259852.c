/*
 * A259852  Numerators of the terms of Lehmer's series S_2(2), where S_k(x) =
 *          Sum_{n>=1} n^k*x^n/binomial(2*n, n).
 * https://oeis.org/A259852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of (n^2 * 2^n) / binom(2n, n)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n) = (n^2 * 2^n) / gcd(n^2 * 2^n, binom(2n, n)) */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

/* Euclidean gcd for arbitrary-precision seq_t types */
static seq_t my_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* compute n^2 * 2^n */
    seq_t num = pow2(n);
    num *= (seq_t)n;
    num *= (seq_t)n;

    /* compute binomial coefficient C(2n, n) */
    idx_t two_n = n + n;
    seq_t den = binomial(two_n, n);

    /* reduce the fraction */
    seq_t g = my_gcd(num, den);
    return num / g;
}
