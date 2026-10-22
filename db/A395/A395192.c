/*
 * A395192  a(n) is the least k with the largest abundancy where k is a proper
 *          divisor of n.
 * https://oeis.org/A395192
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least proper divisor k of n that has the largest
 *            abundancy σ(k)/k. In other words, a(n) is the value n/p where p
 *            is the prime divisor of n that maximises σ(n/p)/(n/p). If
 *            several divisors give the same maximum abundancy, choose the
 *            smallest such divisor (which is guaranteed to be n/p f...
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, sigma_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_prime_u64.h"

/* Return the sequence value a(n) for n ≥ 2.  For n < 2 the value
   is unspecified; we return 0. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    /* factor n into prime powers */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* If n is prime we know the answer immediately. */
    if (cnt == 1 && exps[0] == 1) return (seq_t)1;

    uint64_t best_k     = 0;
    unsigned long long best_sigma = 0;

    for (size_t i = 0; i < cnt; ++i) {
        /* candidate divisor k = n / p_i */
        uint64_t k = (uint64_t)n / primes[i];

        /* compute σ(k) as product over all prime factors */
        unsigned long long sigma_k = 1;
        for (size_t j = 0; j < cnt; ++j) {
            int exp = (int)exps[j] - (j == i ? 1 : 0);
            sigma_k *= sigma_prime_u64(primes[j], exp);
        }

        if (best_k == 0) {
            best_k     = k;
            best_sigma = sigma_k;
            continue;
        }

        /* Compare abundancies: σ(k)/k  vs  best_sigma/best_k
           by cross‑multiplication to avoid division. */
        unsigned long long left  = sigma_k * (unsigned long long)best_k;
        unsigned long long right = best_sigma * (unsigned long long)k;

        if (left > right || (left == right && k < best_k)) {
            best_k = k;
            best_sigma = sigma_k;
        }
    }

    return (seq_t)best_k;
}
