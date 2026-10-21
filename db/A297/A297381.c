/*
 * A297381  Numerator of -A023900(n)/2.
 * https://oeis.org/A297381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of -∏_{p|n} (1 - p) / 2 (∏ over distinct prime
 *            divisors p of n) The product H(n)=∏_{p|n}(1-p) is computed, then
 *            a(n) = (H(n) even? H(n)/-2 : -H(n)). This follows from the
 *            definition a(n)=numerator of -A023900(n)/2, where
 *            A023900(n)=∑_{d|n} μ(d)d and μ is the Möbius function. Sinc...
 * Range:     i8 n <= 256, i16 n <= 9654, i32 n <= 9654, i64 n <= 9654,
 *            i128 n <= 9654, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t H = 1;          /* H(n)   */
    idx_t m = n;          /* temporary copy for factorisation */

    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            /* p is a distinct prime factor of n */
            H *= (seq_t)1 - (seq_t)p;
            while (m % p == 0)   /* remove all powers of p */
                m /= p;
        }
    }

    if (m > 1) /* remaining prime factor > sqrt(original n) */
    {
        H *= (seq_t)1 - (seq_t)m;
    }

    seq_t T = -H;             /* T = -∏(1-p) */

    if (T % 2 == 0)           /* even */
        return T / 2;
    else                      /* odd */
        return T;
}
