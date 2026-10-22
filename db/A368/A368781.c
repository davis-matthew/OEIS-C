/*
 * A368781  The maximal exponent in the unique factorization of n in terms of
 *          distinct "Fermi-Dirac primes".
 * https://oeis.org/A368781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the largest power of 2 that is ≤ any prime exponent in
 *            the prime factorization of n (i.e. the maximum 2^⎣log₂ e⎦ over
 *            all e in n = ∏p^e)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case: n = 1 has no prime factors → a(1) = 0 */
    if (n == (idx_t)1) {
        return (seq_t)0;
    }

    /* Factor n by trial division up to sqrt(n). */
    uint64_t m = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Handle factor 2 */
    if ((m & 1ULL) == 0) {
        uint64_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        primes[cnt] = 2;
        exps[cnt] = e;
        ++cnt;
    }

    /* Handle odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }

    /* Remaining prime > sqrt(original m) */
    if (m > 1) {
        primes[cnt] = m;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Determine the maximum power of 2 that appears in the exponents. */
    seq_t max_pow = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        /* Find the largest power of two ≤ e. */
        seq_t pow = 1;
        while (pow <= (seq_t)e) {
            pow <<= 1;
        }
        pow >>= 1; /* Now pow ≤ e and is a power of two */
        if (pow > max_pow) {
            max_pow = pow;
        }
    }

    return max_pow;
}
