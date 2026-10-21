/*
 * A241663  Number of positive integers k less than or equal to n such that
 *          gcd(k,n) = gcd(k+1,n) = gcd(k+2,n) = gcd(k+3,n) = 1.
 * https://oeis.org/A241663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if n == 1 * 0 if n is divisible by 2 or 3 * prod_{p^e
 *            || n} (p^(e-1) * (p-4)) otherwise * * The product runs over all
 *            prime powers p^e dividing n. * This is the multiplicative
 *            formula for the 4th Schemmel totient * function as given in OEIS
 *            A241663
 * Range:     u8 n <= 262, u16 n <= 9086, u32 n <= 9086, u64 n <= 9086,
 *            u128 n <= 9086, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the trivial case */
    if (n == (idx_t)1) return (seq_t)1;

    uint64_t m = (uint64_t)n;          /* work in 64‑bit for factoring */
    if ((m & 1ULL) == 0ULL || (m % 3ULL) == 0ULL) return (seq_t)0;

    seq_t result = (seq_t)1;

    /* check prime factors of the form 6k±1 up to sqrt(m) */
    for (uint64_t p = 5; p * p <= m; p += 6) {
        /* first prime in the pair */
        if (m % p == 0ULL) {
            uint64_t exp = 0;
            while (m % p == 0ULL) {
                m /= p;
                exp++;
            }
            seq_t pow = (seq_t)1;
            for (uint64_t i = 1; i < exp; ++i) pow *= (seq_t)p;
            result *= pow * (seq_t)(p - 4ULL);
        }

        /* second prime in the pair, p+2 */
        uint64_t p2 = p + 2;
        if (m % p2 == 0ULL) {
            uint64_t exp = 0;
            while (m % p2 == 0ULL) {
                m /= p2;
                exp++;
            }
            seq_t pow = (seq_t)1;
            for (uint64_t i = 1; i < exp; ++i) pow *= (seq_t)p2;
            result *= pow * (seq_t)(p2 - 4ULL);
        }
    }

    /* if anything remains, it is a prime factor > sqrt(original n) */
    if (m > 1ULL) {
        /* exp = 1, so p^(exp-1) = 1 */
        result *= (seq_t)(m - 4ULL);
    }

    return result;
}
