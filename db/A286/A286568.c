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
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/sigma_of.h"

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
    seq_t phi_n = phi(n);               /* Euler's totient of n */
    seq_t sigma_n = sigma_of(n);        /* sum of divisors of n */
    seq_t v2_n = v2_seq(sigma_n);       /* 2‑adic valuation of sigma(n) */

    seq_t t = phi_n + v2_n;             /* k = phi + v2 */
    seq_t prod = t * (t - 1);           /* t*(t-1) */
    seq_t res = (prod / 2) + phi_n;     /* (t*(t-1))/2 + phi */
    return res;
}
