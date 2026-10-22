/*
 * A342476  Numbers m > 1 such that W(m) > W(k) for all 1 < k < m, where W(k)
 *          = omega(k)*log(log(k))/log(k).
 * https://oeis.org/A342476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342476:
   a(n) = 2,3,4,5,6,10,12,14,15,30 for n=1..10
   a(n) = product_{i=1}^{n-7} p_i  (primorial) for n ≥ 10
   (p_i = the i-th prime, 1‑based)
   The following code implements this rule. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static const seq_t _init_seq[9] = {2, 3, 4, 5, 6, 10, 12, 14, 15};

seq_t a(idx_t n)
{
    /* Handle the first nine terms explicitly. */
    if (n <= 9u) {
        return _init_seq[n - 1u];
    }

    /* For n ≥ 10 we need the product of the first (n‑7) primes. */
    idx_t primes_needed = n - 7u;            /* number of primes to multiply */
    /* A safe upper bound for the N-th prime with N ≤ 9993 (≈10⁴) is 200 000. */
    const idx_t LIMIT = 200000u;

    /* Sieve of Eratosthenes up to LIMIT. */
    bool *is_prime = (bool *)malloc((size_t)(LIMIT + 1) * sizeof(bool));
    if (!is_prime) {
        return (seq_t)0;                     /* fallback on failure */
    }
    for (idx_t i = 0u; i <= LIMIT; i++) {
        is_prime[i] = true;
    }
    is_prime[0u] = is_prime[1u] = false;
    for (idx_t p = 2u; p * p <= LIMIT; p++) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= LIMIT; j += p) {
                is_prime[j] = false;
            }
        }
    }

    /* Collect primes until we have enough. */
    uint64_t *primes = NULL;
    size_t    prime_cap = 0;
    size_t    prime_cnt = 0;
    for (idx_t i = 2u; i <= LIMIT && prime_cnt < primes_needed; i++) {
        if (is_prime[i]) {
            if (prime_cnt >= prime_cap) {
                prime_cap = (prime_cap == 0u) ? 16u : prime_cap * 2u;
                uint64_t *tmp = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                if (!tmp) {
                    free(is_prime);
                    free(primes);
                    return (seq_t)0;
                }
                primes = tmp;
            }
            primes[prime_cnt++] = (uint64_t)i;
        }
    }

    free(is_prime);

    if (prime_cnt < primes_needed) {
        free(primes);
        return (seq_t)0;                     /* not enough primes (should not happen) */
    }

    /* Compute the primorial product. */
    seq_t result = (seq_t)1;
    for (size_t i = 0u; i < primes_needed; i++) {
        result *= (seq_t)primes[i];
    }

    free(primes);
    return result;
}
