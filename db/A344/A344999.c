/*
 * A344999  a(n) = A048250(n) * A345001(n).
 * https://oeis.org/A344999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A048250(n) * A345001(n), where A048250(n) = prod_{p|n}
 *            (p+1) and A345001(n) = sigma(n) + n' - 2n (n' = arithmetic
 *            derivative)
 * Range:     i8 n <= 11, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    uint64_t m = (uint64_t)n;
    seq_t N = (seq_t)n;
    seq_t sig = (seq_t)1;
    seq_t rad = (seq_t)1;   /* sum of squarefree divisors */
    seq_t der = (seq_t)0;

    for (uint64_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            unsigned e = 0;
            seq_t term = (seq_t)1, pw = (seq_t)1;
            while (m % p == 0)
            {
                m /= p;
                ++e;
                pw = pw * (seq_t)p;
                term = term + pw;
            }
            sig = sig * term;
            rad = rad * ((seq_t)p + 1);
            der = der + (N / (seq_t)p) * (seq_t)e;
        }
    }
    if (m > 1)
    {
        sig = sig * ((seq_t)m + 1);
        rad = rad * ((seq_t)m + 1);
        der = der + N / (seq_t)m;
    }

    seq_t b = sig + der - (seq_t)2 * N;
    return rad * b;
}
