/*
 * A341663  a(n) is the number of divisors of prime(n)^3 - 1.
 * https://oeis.org/A341663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of prime(n)^3 - 1 (τ(p_n³ − 1))
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Ensure n >= 1 as per OEIS definition */
    if (n == 0) return 0;

    /* Get the n-th prime */
    uint32_t p = get_prime(n);          /* uint32_t is enough for primes up to >10⁶ */

    /* Compute p³ − 1 (fits in uint64_t for all n <= 10000) */
    uint64_t val = (uint64_t)p * p * p - 1ULL;

    /* Factorize val. 2000 primes are enough for numbers up to ~10¹⁵. */
    static uint64_t primes[2000];
    static uint64_t exps[2000];
    size_t cnt = 0;
    factor_u64(val, primes, exps, &cnt);

    /* Count divisors: product of (exponent + 1) */
    seq_t dcnt = 1;
    for (size_t i = 0; i < cnt; ++i) {
        dcnt *= (seq_t)(exps[i] + 1);
    }

    return dcnt;
}
