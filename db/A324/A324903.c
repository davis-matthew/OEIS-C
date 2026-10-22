/*
 * A324903  a(n) = 1 if A007814(sigma(n)) > A007814(n), 0 otherwise. Here
 *          A007814(n) gives the 2-adic valuation of n.
 * https://oeis.org/A324903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A007814(sigma(n)) > A007814(n), 0 otherwise
 * Range:     u8 n <= 5092, u16 n <= 9243, u32 n <= 9243, u64 n <= 9243,
 *            u128 n <= 9243, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here A007814 is the 2‑adic valuation (exponent of 2) and sigma is the sum of divisors. */

#include "../utils/sigma_of.h"

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
