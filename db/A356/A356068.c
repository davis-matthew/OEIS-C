/*
 * A356068  Number of integers ranging from 1 to n that are not prime-powers
 *          (1 is not a prime-power).
 * https://oeis.org/A356068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (# of prime powers <= n)
 * Range:     u8 n <= 22, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Count integers in [1,n] that are not prime‑powers (1 is not a prime‑power). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    size_t N = (size_t)n;

    /* Sieve of Eratosthenes up to N */
    bool *is_prime = (bool*)malloc((N + 1) * sizeof(bool));
    if (!is_prime) return 0;
    for (size_t i = 0; i <= N; ++i) is_prime[i] = true;
    if (N >= 0) {
        is_prime[0] = false;
        if (N >= 1) is_prime[1] = false;
    }
    for (size_t i = 2; i * i <= N; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= N; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* Gather the primes */
    size_t prime_cnt = 0;
    for (size_t i = 2; i <= N; ++i) {
        if (is_prime[i]) ++prime_cnt;
    }
    uint64_t *primes = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return 0;
    }
    size_t idx = 0;
    for (size_t i = 2; i <= N; ++i) {
        if (is_prime[i]) {
            primes[idx++] = (uint64_t)i;
        }
    }

    /* Count prime powers ≤ n */
    seq_t prime_power_cnt = 0;
    seq_t limit = (seq_t)n;
    for (size_t k = 0; k < prime_cnt; ++k) {
        uint64_t p = primes[k];
        prime_power_cnt += 1;                     /* p^1 */
        seq_t power = (seq_t)p * (seq_t)p;        /* start with p^2 */
        while (power <= limit) {
            prime_power_cnt += 1;
            power *= (seq_t)p;
        }
    }

    seq_t result = limit - prime_power_cnt;      /* non‑prime‑powers count */

    free(is_prime);
    free(primes);
    return result;
}
