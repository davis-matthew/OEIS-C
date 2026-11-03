/*
 * A325030  a(n) = Product_{d|n} (sigma(d)*pod(d)) where sigma(k) = the sum of
 *          the divisors of k (A000203) and pod(k) = the product of the
 *          divisors of k (A007955).
 * https://oeis.org/A325030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} (sigma(d)*pod(d))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 11, u64 n <= 23, u128 n <= 59,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 893 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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

/* Compute the product of the divisors of d (pod(d)).  Simple square‑root
   enumeration is fine because d <= 10000 in the relevant tests. */
static seq_t _pod_of(uint64_t d)
{
    seq_t prod = 1;
    for (uint64_t i = 1; i * i <= d; ++i) {
        if (d % i == 0) {
            prod *= (seq_t)i;
            uint64_t j = d / i;
            if (j != i) prod *= (seq_t)j;
        }
    }
    return prod;
}

seq_t a(idx_t n)
{
    uint64_t un = (uint64_t)n;   /* n is always ≥ 1 */
    seq_t result = 1;

    for (uint64_t d = 1; d * d <= un; ++d) {
        if (un % d == 0) {
            seq_t sigma_d = sigma_of((idx_t)d);
            seq_t pod_d   = _pod_of(d);
            result *= sigma_d * pod_d;

            uint64_t e = un / d;
            if (e != d) {
                seq_t sigma_e = sigma_of((idx_t)e);
                seq_t pod_e   = _pod_of(e);
                result *= sigma_e * pod_e;
            }
        }
    }
    return result;
}
