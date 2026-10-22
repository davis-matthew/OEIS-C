/*
 * A306745  Even bisection of A318256.
 * https://oeis.org/A306745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 89, u64 n <= 209,
 *            u128 n <= 899, big any n
 * Helpers:   binom_not_divisible_by_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (denominator of the Bernoulli polynomial B_{2n}(x)) /
 *                (squarefree kernel of 2n+1)
 *
 * Computation:
 *   * Primes are generated up to 2n+1 by a simple sieve.
 *   * The squarefree kernel (radical) of 2n+1 is obtained by factoring that
 *     number with the primes.
 *   * For every prime p ≤ 2n+1 that is not a radical factor, we test whether
 *     p actually appears in the denominator of B_{2n}(x):
 *        * We search for an even j with 0 < j ≤ 2n, p‑1 | j, and
 *          binomial(2n, j) not divisible by p.
 *        * If such a j exists, p survives in the denominator.
 *   * The product of all surviving primes that are not radical factors is
 *     exactly a(n).
 *
 * This implementation is purely integer and uses only the allowed
 * operators and helper functions.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binom_not_divisible_by_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t N = 2 * n;          /* 2n */
    idx_t limit = N + 1;      /* we need primes up to 2n+1 */

    /* --- Sieve of Eratosthenes up to limit --- */
    bool *isPrime = (bool *)malloc((limit + 1) * sizeof(bool));
    bool *radFlag  = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!isPrime || !radFlag) return (seq_t)0;  /* allocation failure safety */

    for (idx_t i = 0; i <= limit; ++i) {
        isPrime[i] = true;
        radFlag[i] = false;
    }
    if (limit >= 0)  isPrime[0] = false;
    if (limit >= 1)  isPrime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (isPrime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                isPrime[j] = false;
        }
    }

    /* Build prime list */
    idx_t *primes = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    idx_t primeCnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (isPrime[i]) primes[primeCnt++] = i;

    /* Compute radical (squarefree kernel) of 2n+1 */
    idx_t m = 2 * n + 1;
    idx_t tmp = m;
    for (idx_t i = 0; i < primeCnt && primes[i] * primes[i] <= tmp; ++i) {
        idx_t p = primes[i];
        if (tmp % p == 0) {
            radFlag[p] = true;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) radFlag[tmp] = true;   /* remaining prime factor */

    /* Compute the final value */
    seq_t result = 1;
    for (idx_t s = 0; s < primeCnt; ++s) {
        idx_t p = primes[s];
        if (radFlag[p]) continue;  /* radical primes cancel */

        bool survive = false;

        if (p == 2) {
            /* j must be even and >0 */
            for (idx_t j = 2; j <= N; j += 2) {
                if (binom_not_divisible_by_prime(N, j, p)) {
                    survive = true;
                    break;
                }
            }
        } else {
            /* p-1 is even, hence all multiples are even */
            for (idx_t j = p - 1; j <= N; j += p - 1) {
                if (binom_not_divisible_by_prime(N, j, p)) {
                    survive = true;
                    break;
                }
            }
        }

        if (survive) result *= (seq_t)p;
    }

    free(isPrime);
    free(radFlag);
    free(primes);
    return result;
}
