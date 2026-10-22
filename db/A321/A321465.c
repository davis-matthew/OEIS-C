/*
 * A321465  Expansion of (phi(x)^3 / phi(x^3))^2 in powers of x where phi() is
 *          a Ramanujan theta function.
 * https://oeis.org/A321465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 12*(σ(n) + 2*σ(n/2) + 9*σ(n/3) + 4*σ(n/4) - 54*σ(n/6) +
 *            36*σ(n/12)) for n>0 a(0) = 1
 * Range:     i8 n <= 2, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Sum of divisors of m (m >= 1).  Uses trial division; safe for m <= 10^7 on 64‑bit builds. */
static seq_t sigma_u64(uint64_t m)
{
    seq_t res = 1;
    uint64_t d = 2;
    while (d * d <= m) {
        if (m % d == 0) {
            seq_t d_pow = 1;
            seq_t term = 1;
            while (m % d == 0) {
                m /= d;
                d_pow *= (seq_t)d;
                term += d_pow;
            }
            res *= term;
        }
        d++;
    }
    if (m > 1) {                     /* remaining prime */
        res *= (seq_t)(1 + (seq_t)m);
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;              /* a(0) */
    uint64_t nn = (uint64_t)n;

    seq_t s1 = sigma_u64(nn);
    seq_t s2 = 0, s3 = 0, s4 = 0, s6 = 0, s12 = 0;

    if ((nn & 1) == 0)               /* n even */
        s2 = sigma_u64(nn / 2);
    if (nn % 3 == 0)
        s3 = sigma_u64(nn / 3);
    if ((nn & 3) == 0)               /* n divisible by 4 */
        s4 = sigma_u64(nn / 4);
    if ((nn % 6) == 0)
        s6 = sigma_u64(nn / 6);
    if ((nn % 12) == 0)
        s12 = sigma_u64(nn / 12);

    seq_t val = (seq_t)12 *
        ( s1
          + (seq_t)2 * s2
          + (seq_t)9 * s3
          + (seq_t)4 * s4
          - (seq_t)54 * s6
          + (seq_t)36 * s12 );
    return val;
}
