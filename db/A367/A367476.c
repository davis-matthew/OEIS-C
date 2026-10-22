/*
 * A367476  Sum of the final digits of the distinct prime divisors of n.
 * https://oeis.org/A367476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{p|n, p prime} (p mod 10) (sum over distinct prime
 *            divisors)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  The first term is 0 for n = 1. */
    if (n <= 1)
        return 0;

    seq_t sum = 0;
    uint64_t m = (uint64_t)n;          /* work on a 64‑bit copy for safety */

    /* handle factor 2 */
    if ((m & 1ULL) == 0) {
        sum += (seq_t)2;                /* 2 mod 10 = 2 */
        while ((m & 1ULL) == 0)
            m >>= 1;
    }

    /* test odd candidates up to sqrt(m) */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            sum += (seq_t)(p % 10);
            while (m % p == 0)
                m /= p;
        }
    }

    /* if a prime factor > sqrt(original n) remains, it is the last one */
    if (m > 1) {
        sum += (seq_t)(m % 10);
    }

    return sum;
}
