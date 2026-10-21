/*
 * A289613  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (12, 307) or the same sequence
 *          for the mesh pattern (12, 409).
 * https://oeis.org/A289613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n) - (n^2 - n + 2)/2 for n > 4, a(0)=1, a(1)=1, a(2)=1,
 *            a(3)=1, a(4)=8 (C(n): Catalan number)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean gcd for seq_t.  Works for unsigned or arbitrary-precision types. */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* binom(2*n, n) without overflow or loss of precision. */
static seq_t binom_2n_n(idx_t n)
{
    seq_t res = (seq_t)1;
    seq_t sn  = (seq_t)n;
    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        seq_t num = sn + (seq_t)i;          /* n + i   */
        seq_t den = (seq_t)i;              /* i       */

        /* Cancel common factors between numerator and denominator. */
        seq_t g1 = gcd_seq(num, den);
        num   /= g1;
        den   /= g1;

        /* Cancel factors between current result and remaining denominator. */
        seq_t g2 = gcd_seq(res, den);
        res   /= g2;
        den   /= g2;

        /* Multiply and divide, reading guaranteed to be exact. */
        res = res * num;
        res = res / den;
    }
    return res;
}

/* Catalan number C(n) = binom(2*n, n)/(n+1)  (integer division). */
static seq_t catalan_n(idx_t n)
{
    seq_t bin = binom_2n_n(n);
    seq_t sn  = (seq_t)n;
    return bin / (sn + (seq_t)1);
}

/* Main OEIS function. */
seq_t a(idx_t n)
{
    static const seq_t init[5] = { (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)8 };
    if (n < (idx_t)5) return init[n];

    seq_t sn   = (seq_t)n;
    seq_t sub  = (sn * sn - sn + (seq_t)2) / (seq_t)2;
    seq_t cat  = catalan_n(n);
    return cat - sub;
}
