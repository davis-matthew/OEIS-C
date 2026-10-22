/*
 * A359211  a(n) = tau(3*n-1)/2, where tau(n) = number of divisors of n, cf.
 *          A000005.
 * https://oeis.org/A359211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(3*n - 1)/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau(m) = number of positive divisors of m. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* not used, but defensive */
        return 0;

    /* Compute m = 3*n - 1.  The product is modulo 2^64, which is fine
       for the range we need (n up to 10^4 and even a bit larger). */
    uint64_t m = (uint64_t)3 * n - 1;

    seq_t tau = 1;          /* product of (exponent+1) for each prime factor */

    /* Count factor 2 */
    unsigned int cnt = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        cnt++;
    }
    if (cnt)
        tau *= (seq_t)(cnt + 1);

    /* Count odd prime factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            cnt = 0;
            while (m % p == 0) {
                m /= p;
                cnt++;
            }
            tau *= (seq_t)(cnt + 1);
        }
    }

    /* If a prime factor > sqrt(original m) remains */
    if (m > 1)
        tau *= 2;

    /* a(n) is tau divided by 2 (tau is always even). */
    return tau / 2;
}
