/*
 * A305461  The number of one-digit numbers, k, in base n such that k^2 and
 *          k^3 end in the same digit.
 * https://oeis.org/A305461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} (p^(⌊e/2⌋) + 1) (multiplicative)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* A static cache of primes up to 1 000 000 (enough for n ≤ 10^12). */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    if (!primes) {
        const idx_t LIMIT = 1000000ULL;
        build_primes_upto(LIMIT, &primes, &prime_cnt);
    }

    idx_t m = n;
    seq_t prod = 1;

    /* Factor using the cached primes. */
    for (size_t i = 0; i < prime_cnt && (idx_t)primes[i] * primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            size_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* Compute p^(floor(exp/2)) */
            seq_t pe_half = 1;
            for (size_t j = 0; j < exp / 2; ++j)
                pe_half *= (seq_t)p;
            prod *= (pe_half + (seq_t)1);
        }
    }

    /* If a prime factor larger than sqrt(original n) remains, its
       exponent must be 1, so the factor contributes (p^0 + 1) = 2. */
    if (m > 1)
        prod *= (seq_t)2;

    return prod;
}
