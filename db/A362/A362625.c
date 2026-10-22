/*
 * A362625  a(n) = Sum_{k not divides n - k, 0 <= k < n} k.
 * https://oeis.org/A362625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)/2 - n*tau(n) + sigma(n)
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for out‑of‑range calls */
    seq_t N = (seq_t)n;
    /* number of divisors tau(n) */
    seq_t tau = (seq_t)divisor_count_u64(n);
    /* sum of divisors sigma(n) */
    seq_t sigma = sigma_of(n);
    /* compute N*(N-1)/2  */
    seq_t half = N * (N - (seq_t)1) / (seq_t)2;
    /* final formula */
    return half - N * tau + sigma;
}
