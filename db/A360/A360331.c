/*
 * A360331  a(n) is the sum of divisors of n that have only prime factors that
 *          are not prime-indexed primes.
 * https://oeis.org/A360331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of σ(p^e) over all prime powers p^e dividing n
 *            with p a non‑prime‑indexed prime, where σ(p^e) =
 *            (p^(e+1)−1)/(p−1). Prime‑indexed primes (primes whose 1‑based
 *            index is prime) contribute a factor 1
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* Cached list of primes (increasing).  */
static uint64_t *primes_cache = NULL;
static size_t   primes_cnt   = 0;
static idx_t    primes_limit = 0;

/* Ensure the prime list contains all primes ≤ limit.  */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= primes_limit) return;
    if (primes_cache) free(primes_cache);
    build_primes_upto(limit, &primes_cache, &primes_cnt);
    primes_limit = limit;
}

/* Return the zero‑based index of prime p in primes_cache.
   Linear search is fine for the sizes we deal with.  */
static size_t prime_index(uint64_t p)
{
    for (size_t i = 0; i < primes_cnt; ++i)
        if (primes_cache[i] == p) return i;
    /* Should never happen for valid input.  */
    return primes_cnt;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;     /* a(1) = 1 */
    /* Ensure primes up to n so all prime factors are known.  */
    ensure_primes_up_to(n);

    uint64_t temp = (uint64_t)n;
    seq_t result = 1;

    /* Trial division by the cached primes.  */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes_cache[i];
        if (p > temp / p) break;           /* p * p > temp: stop */
        if (temp % p == 0) {
            /* Count exponent e of prime p.  */
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* Check whether p is a prime‑indexed prime.  */
            if (!is_prime((idx_t)(i + 1))) {
                /* Compute σ(p^e) = 1 + p + p² + … + p^e  */
                seq_t sum = 1;
                seq_t pow = 1;
                for (unsigned k = 0; k < e; ++k) {
                    pow *= (seq_t)p;
                    sum += pow;
                }
                result *= sum;
            }   /* else multiply by 1, i.e. ignore */
        }
    }

    /* Remaining factor > 1 is a prime > sqrt(original n).  */
    if (temp > 1) {
        uint64_t p = temp;
        size_t idx = prime_index(p);
        if (!is_prime((idx_t)(idx + 1))) {
            result *= (seq_t)(1 + p);
        }   /* else factor contributes 1 */
    }

    return result;
}
