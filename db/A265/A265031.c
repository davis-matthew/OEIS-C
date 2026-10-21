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
 * Helpers:   gcd, power_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/power_u.h"

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
