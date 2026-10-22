/*
 * A353496  The arithmetic derivative of the largest proper divisor of n,
 *          reduced modulo 4, with a(1) = 0.
 * https://oeis.org/A353496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic derivative of the largest proper divisor of n,
 *            reduced modulo 4
 * Range:     u8 n <= 9945, u16 n <= 9945, u32 n <= 9945, u64 n <= 9945,
 *            u128 n <= 9945, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) {
        return 0;
    }

    /* Find smallest prime factor of n */
    uint64_t spf = 2;
    uint64_t tmp = (uint64_t)n;
    if (tmp % 2 != 0) {
        spf = (uint64_t)n; /* assume prime until proven composite */
        for (uint64_t d = 3; d * d <= tmp; d += 2) {
            if (tmp % d == 0) {
                spf = d;
                break;
            }
        }
    }

    /* Largest proper divisor m = n / spf */
    uint64_t m = ((uint64_t)n) / spf;
    if (m == 1) {
        return 0;
    }

    /* Compute arithmetic derivative of m:
       d(m) = m * sum_{p^e || m} e/p  = sum_{p^e || m} (m/p)*e
    */
    seq_t sum = 0;
    uint64_t m_orig = m;      /* keep original value for m/p */
    uint64_t m_copy = m;      /* mutable copy for factoring */

    /* factor 2 */
    uint64_t e = 0;
    while (m_copy % 2 == 0) {
        m_copy /= 2;
        e += 1;
    }
    if (e != 0) {
        seq_t term = ((seq_t)(m_orig / 2)) * ((seq_t)e);
        sum += term;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m_copy; p += 2) {
        if (m_copy % p != 0) continue;
        e = 0;
        while (m_copy % p == 0) {
            m_copy /= p;
            e += 1;
        }
        seq_t term = ((seq_t)(m_orig / p)) * ((seq_t)e);
        sum += term;
    }

    /* if m_copy > 1, it is a prime factor with exponent 1 */
    if (m_copy > 1) {
        seq_t term = ((seq_t)(m_orig / m_copy)) * (seq_t)1;
        sum += term;
    }

    return sum % 4;
}
