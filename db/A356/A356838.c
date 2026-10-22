/*
 * A356838  The smallest of the most common prime factors of n.
 * https://oeis.org/A356838
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime factor of n having the maximum exponent in
 *            n's prime factorization
 * Range:     u8 n <= 256, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;      /* undefined for n < 2, but harmless */
    uint64_t temp = (uint64_t)n;
    uint64_t maxExp = 0;
    uint64_t result = 0;
    uint64_t e = 0;

    /* factor 2 */
    while ((temp & 1) == 0) {                     /* temp % 2 == 0 */
        temp >>= 1;                               /* divide by 2 */
        ++e;
    }
    if (e > 0) {
        maxExp = e;
        result = 2;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e > maxExp) {
                maxExp = e;
                result = p;
            } else if (e == maxExp && p < result) {
                result = p;
            }
        }
    }

    /* remaining prime factor > 1 */
    if (temp > 1) {
        e = 1;
        if (e > maxExp) {
            maxExp = e;
            result = temp;
        } else if (e == maxExp && temp < result) {
            result = temp;
        }
    }

    return (seq_t)result;
}
