/*
 * A362625  a(n) = Sum_{k not divides n - k, 0 <= k < n} k.
 * https://oeis.org/A362625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)/2 - n*tau(n) + sigma(n)
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"

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
    if (n == 0) return (seq_t)0;          /* safety for out‑of‑range calls */
    seq_t N = (seq_t)n;
    /* number of divisors tau(n) */
    seq_t tau = (seq_t)num_divisors(n);
    /* sum of divisors sigma(n) */
    seq_t sigma = sigma_of(n);
    /* compute N*(N-1)/2  */
    seq_t half = N * (N - (seq_t)1) / (seq_t)2;
    /* final formula */
    return half - N * tau + sigma;
}
