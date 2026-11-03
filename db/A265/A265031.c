/*
 * A265031  Denominator of Kirchhoff index of ladder graph L_n.
 * https://oeis.org/A265031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of k(n) where k(n) = (1/3)*n^2*( n -
 *            sqrt(3)/tanh(n*log(2-sqrt(3))) ). The code uses the
 *            representation (2+√3)^n = P + Q√3, then k(n) = n^2*(n*Q+P) /
 *            (3*Q) and the denominator is reduced
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* compute (P,Q) such that (2+√3)^n = P + Q√3 using binary exponentiation */
static void power_u(idx_t n, seq_t *P, seq_t *Q)
{
    seq_t a = (seq_t)2;   /* P part of base (2+√3) */
    seq_t b = (seq_t)1;   /* Q part of base (2+√3) */
    *P = (seq_t)1;        /* (2+√3)^0 = 1 + 0√3 */
    *Q = (seq_t)0;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            /* result *= base */
            seq_t newP = (*P) * a + (*Q) * b * (seq_t)3;
            seq_t newQ = (*P) * b + (*Q) * a;
            *P = newP;
            *Q = newQ;
        }
        /* base *= base */
        seq_t newa = a * a + b * b * (seq_t)3;
        seq_t newb = a * b * (seq_t)2;
        a = newa;
        b = newb;
        exp >>= 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence is defined for n≥1 */
    seq_t P, Q;
    power_u(n, &P, &Q);
    seq_t n_seq = (seq_t)n;
    seq_t nq_plus_p = n_seq * Q + P;
    seq_t n_sq = n_seq * n_seq;
    seq_t numerator = n_sq * nq_plus_p;
    seq_t denominator = Q * (seq_t)3;
    seq_t g = gcd(numerator, denominator);
    denominator /= g;
    return denominator;
}
