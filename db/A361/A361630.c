/*
 * A361630  a(n) is the numerator of the median of the distinct prime factors
 *          of n.
 * https://oeis.org/A361630
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A361630: numerator of the median of the distinct prime factors of n */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset – behavior is irrelevant. */
    if (n < 2)
        return (seq_t)0;

    /* Factor n and keep each distinct prime once. */
    uint64_t m = (uint64_t)n;
    uint64_t primes[16];          /* 16 > sufficient for n ≤ 10⁴ */
    int count = 0;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            if (count < 16)
                primes[count++] = p;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1 && count < 16)
        primes[count++] = m;

    /* Compute the numerator of the median. */
    if (count % 2 == 1) {          /* odd number of primes */
        return (seq_t)primes[count / 2];
    } else {                       /* even number of primes */
        uint64_t sum = primes[count / 2 - 1] + primes[count / 2];
        /* Reduce the fraction (sum/2). */
        return (seq_t)((sum % 2 == 0) ? sum / 2 : sum);
    }
}
