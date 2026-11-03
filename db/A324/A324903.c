/*
 * A324903  a(n) = 1 if A007814(sigma(n)) > A007814(n), 0 otherwise. Here
 *          A007814(n) gives the 2-adic valuation of n.
 * https://oeis.org/A324903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A007814(sigma(n)) > A007814(n), 0 otherwise
 * Range:     u8 n <= 5092, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9243 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here A007814 is the 2‑adic valuation (exponent of 2) and sigma is the sum of divisors. */

#include <stdlib.h>

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

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* compute sum of divisors of n */
    seq_t sigma = sigma_of(n);

    /* 2‑adic valuation of n (number of trailing zero bits) */
    idx_t   tmp_n = n;
    int v2_n = 0;
    while ((tmp_n & 1ull) == 0) {
        ++v2_n;
        tmp_n >>= 1ull;
    }

    /* 2‑adic valuation of sigma */
    seq_t tmp_sigma = sigma;
    int v2_sigma = 0;
    while ((tmp_sigma & 1) == 0) {
        ++v2_sigma;
        tmp_sigma >>= 1;
    }

    return (v2_sigma > v2_n) ? 1 : 0;
}
