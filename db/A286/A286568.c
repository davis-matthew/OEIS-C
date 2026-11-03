/*
 * A286568  Compound filter (phi(n) & 2-adic valuation of sigma(n)): a(n) =
 *          P(A000010(n), A286357(n)), where P(n,k) is sequence A000027 used
 *          as a pairing function.
 * https://oeis.org/A286568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((A000010(n)+A286357(n))*(A000010(n)+A286357(n)-1))/2 +
 *            A000010(n)
 * Range:     u8 n <= 16, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 8801 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/totient.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Count the exponent of 2 dividing x. */
static seq_t v2_seq(seq_t x)
{
    seq_t cnt = 0;
    while ((x & 1) == 0)
    {
        x >>= 1;
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t phi_n = totient(n);               /* Euler's totient of n */
    seq_t sigma_n = sigma_of(n);        /* sum of divisors of n */
    seq_t v2_n = v2_seq(sigma_n);       /* 2‑adic valuation of sigma(n) */

    seq_t t = phi_n + v2_n;             /* k = phi + v2 */
    seq_t prod = t * (t - 1);           /* t*(t-1) */
    seq_t res = (prod / 2) + phi_n;     /* (t*(t-1))/2 + phi */
    return res;
}
