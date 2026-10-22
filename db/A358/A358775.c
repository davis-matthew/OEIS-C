/*
 * A358775  a(n) = 1 if the prime factorization of n has an even number of
 *          prime factors that sum to an odd number, otherwise 0.
 * https://oeis.org/A358775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the number of prime factors of n (counting
 *            multiplicity) * is even and the sum of all prime factors
 *            (counting multiplicity) is odd; * otherwise a(n) = 0
 * Range:     u8 n <= 9813, u16 n <= 9813, u32 n <= 9813, u64 n <= 9813,
 *            u128 n <= 9813, big any n
 * Helpers:   bigomega, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bigomega.h"          /* bigomega(idx_t) → seq_t */
#include "../utils/factor_u64.h"        /* factor_u64(uint64_t n,
                                          uint64_t *primes,
                                          uint64_t *exponents,
                                          size_t *cnt) */

seq_t a(idx_t n)
{
    /* Count total prime factors (Ω(n)). */
    seq_t omega = bigomega(n);
    if ((omega & 1) != 0)   /* Ω(n) is odd → a(n) = 0 */
        return 0;

    /* For n == 1 there are no prime factors; sum is 0 (even). */
    if (n <= 1)
        return 0;

    /* Factor n and examine odd prime exponents. */
    static const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Parity of the sum of prime factors = parity of sum of exponents of
     * odd primes only, because 2 contributes an even quantity to the sum. */
    int sum_parity = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if ((primes[i] & 1U) != 0)        /* odd prime */
            sum_parity ^= (int)(exps[i] & 1U);
    }

    /* a(n) = 1 iff Omega(n) even (checked above) AND sum_parity is odd. */
    return (sum_parity & 1) ? (seq_t)1 : (seq_t)0;
}
