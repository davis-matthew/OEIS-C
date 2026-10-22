/*
 * A324891  a(n) = sigma(A170818(n)), where A170818(n) is the part of n
 *          composed of prime factors of form 4k+1.
 * https://oeis.org/A324891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma(A170818(n)) * Implementation: factor n, keep only
 *            primes ≡ 1 (mod 4), * compute the sum of divisors of that
 *            prime‑part
 * Range:     u8 n <= 256, u16 n <= 7900, u32 n <= 7900, u64 n <= 7900,
 *            u128 n <= 7900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are defined in the build environment */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset begins at 1 */
    uint64_t m = (uint64_t)n;
    if (m == 1) return (seq_t)1;          /* a(1) = 1 */

    seq_t result = (seq_t)1;

    /* remove all factors of 2 – they never contribute */
    while ((m & 1ULL) == 0) m >>= 1;

    /* trial divide by odd primes up to sqrt(m) */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p) continue;              /* p does not divide m */

        /* factor p out of m and count exponent */
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }

        if (p % 4 == 1) {
            /* sigma(p^exp) = 1 + p + p^2 + ... + p^exp */
            seq_t sigma = (seq_t)1;
            seq_t term  = (seq_t)p;
            for (int i = 1; i <= exp; ++i) {
                sigma += term;
                term  *= (seq_t)p;
            }
            result *= sigma;
        }
        /* else prime factor ≡ 3 (mod 4) → ignored */
    }

    /* remaining prime factor (if any) */
    if (m > 1 && (m % 4 == 1)) {
        /* exponent is 1, sigma(m) = m + 1 */
        result *= (seq_t)(m + 1);
    }

    return result;
}
