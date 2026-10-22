/*
 * A343489  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          T(n,k) = Sum_{j=1..n} k^(gcd(j, n) - 1).
 * https://oeis.org/A343489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) with i+j = n, where T(n,k)=∑_{d|n}φ(n/d)·k^(d−1)
 *            (and T(0,k)=0). The flat index n is obtained by reading the
 *            square array by antidiagonals. This implementation uses only
 *            integer arithmetic and should run in <2 s for n≈10000
 * Range:     u8 n <= 49, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* compute φ(m) by trial division */
static seq_t phi_of(idx_t m)
{
    if (m <= 1) return (seq_t)m;
    seq_t result = (seq_t)m;
    idx_t t = m;
    for (idx_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            while (t % p == 0) t /= p;
            result -= result / p;
        }
    }
    if (t > 1) result -= result / t;
    return result;
}

/* integer exponentiation: k^e */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp)
    {
        if (exp & 1) result *= cur;
        exp >>= 1;
        if (exp) cur *= cur;
    }
    return result;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* find diagonal m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2 */
    long double disc = sqrtl((long double)8 * n + 1);
    idx_t m = ((idx_t)disc - 1) / 2;

    /* adjust to guarantee correct bounds (avoids floating‑point errors) */
    idx_t tri = m * (m + 1) / 2;
    while (tri > n) { --m; tri = m * (m + 1) / 2; }
    while ((m + 1) * (m + 2) / 2 <= n) ++m;

    idx_t i = n - tri;      /* row index */
    idx_t j = m - i;        /* column index */

    if (i == 0) return 0;   /* T(0,k)=0 */

    seq_t sum = 0;
    idx_t lim = (idx_t)sqrtl((long double)i);
    for (idx_t d = 1; d <= lim; ++d)
    {
        if (i % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = i / d;
            /* divisor d1 */
            seq_t phi1 = phi_of(i / d1);
            seq_t pw1  = pow_seq((seq_t)j, d1 - 1);
            sum += phi1 * pw1;
            if (d1 != d2)
            {
                /* divisor d2 */
                seq_t phi2 = phi_of(i / d2);
                seq_t pw2  = pow_seq((seq_t)j, d2 - 1);
                sum += phi2 * pw2;
            }
        }
    }
    return sum;
}
