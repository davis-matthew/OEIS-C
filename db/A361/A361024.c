/*
 * A361024  a(n) = 1 if n and sigma(n) have equal 2-adic valuations, otherwise
 *          0, where sigma is the sum of divisors function.
 * https://oeis.org/A361024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n and sigma(n) have equal 2‑adic valuations, else 0
 * Range:     u8 n <= 7496, u16 n <= 9853, u32 n <= 9853, u64 n <= 9853,
 *            u128 n <= 9853, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 0 is not a valid index for OEIS offset 1; return 0 for safety. */
    if (n == 0) return 0;

    /* Compute v2(n) – number of trailing zeros in binary representation. */
    int v2_n = 0;
    idx_t tmp_n = n;
    while ((tmp_n & 1) == 0) {
        tmp_n >>= 1;
        ++v2_n;
    }

    /* Calculate sigma(n) = sum of divisors of n.  */
    seq_t sigma = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            sigma += (seq_t)i;                     /* divisor i */
            idx_t j = n / i;
            if (j != i) sigma += (seq_t)j;         /* divisor j (different) */
        }
    }

    /* Compute v2(sigma).  */
    int v2_sigma = 0;
    seq_t tmp_sigma = sigma;
    while ((tmp_sigma & 1) == 0) {
        tmp_sigma >>= 1;
        ++v2_sigma;
    }

    /* Return 1 if the 2‑adic valuations match, otherwise 0. */
    return (v2_n == v2_sigma) ? (seq_t)1 : (seq_t)0;
}
