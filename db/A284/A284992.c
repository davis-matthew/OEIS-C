/*
 * A284992  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of Product_{j>=1} (1+x^j)^(j^k) in
 *          powers of x.
 * https://oeis.org/A284992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^i in ∏_{j≥1}(1+x^j)^{j^k} where (i,k)
 *            are the antidiagonal coordinates of n
 * Range:     u8 n <= 38, u16 n <= 70, u32 n <= 142, u64 n <= 284,
 *            u128 n <= 541, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_pow.h"

/* Integer square‑root for 64‑bit values (floor). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    int i;
    for (i = 63; i >= 0; --i)
    {
        idx_t b = r | ((idx_t)1 << i);
        if (n / b >= b)
            r = b;
    }
    return r;
}

/* Compute a(n) for the OEIS sequence A284992. */
seq_t a(idx_t n)
{
    /* Determine antidiagonal coordinates (i,k). */
    idx_t tri = 8 * n + 1;                /* 8n+1 */
    idx_t s = isqrt_u64(tri);             /* floor(sqrt(8n+1))   */
    idx_t d = (s - 1) / 2;                /* floor((√(8n+1)-1)/2) */
    idx_t prev = d * (d + 1) / 2;
    idx_t i = n - prev;                   /* row index  (degree)  */
    idx_t k = d - i;                      /* column index (power) */

    if (i == 0)
        return (seq_t)1;                  /* first column always 1 */

    /* Maximum degree needed. For n<=10000, i<=140; 200 is a safe margin. */
    const size_t BUF = 200;
    seq_t poly[BUF];
    seq_t newpoly[BUF];
    seq_t bin[BUF];

    /* Initialize polynomial P(x) = 1. */
    poly[0] = 1;
    for (size_t t = 1; t <= (size_t)i; ++t)
        poly[t] = 0;

    /* Multiply by (1 + x^t)^{t^k} for t = 1 .. i. */
    for (idx_t t = 1; t <= i; ++t)
    {
        /* Exponent e = t^k. */
        seq_t e = seq_pow((seq_t)t, k);

        /* Highest power of x needed from this factor: t * q <= i. */
        idx_t tmax = i / t;
        if (tmax > i)
            tmax = i;

        /* Binomial coefficients for (1 + x^t)^e up to degree tmax. */
        bin[0] = 1;
        for (idx_t m = 1; m <= tmax; ++m)
        {
            seq_t numer = e - (seq_t)(m - 1);
            bin[m] = (bin[m - 1] * numer) / (seq_t)m;
        }

        /* Clear new polynomial. */
        for (size_t z = 0; z <= (size_t)i; ++z)
            newpoly[z] = 0;

        /* Convolution: multiply current poly by the binomial expansion. */
        for (size_t pos = 0; pos <= (size_t)i; ++pos)
        {
            if (poly[pos] == 0)
                continue;
            for (idx_t m = 0; m <= tmax; ++m)
            {
                size_t idx = pos + (size_t)t * m;
                if (idx > i)
                    break;
                newpoly[idx] += poly[pos] * bin[m];
            }
        }

        /* Copy back to poly for next iteration. */
        for (size_t z = 0; z <= (size_t)i; ++z)
            poly[z] = newpoly[z];
    }

    return poly[(size_t)i];
}
