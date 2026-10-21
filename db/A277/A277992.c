/*
 * A277992  b(n, 2) where b(n, m) is defined by expansion of ((Product_{k>=1}
 *          (1 - x^(prime(n)*k))/(1 - x^k)^prime(n)) - 1)/prime(n) in powers
 *          of x.
 * https://oeis.org/A277992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 for n = 1; a(n) = (prime(n) + 3) / 2 for n > 1
 * Range:     u8 n <= 54, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* prime(n) is the n‑th prime, 1‑based (2,3,5,7,…)   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache of primes.  prime_cache[i] is the (i+1)-th prime. */
static seq_t *prime_cache = NULL;
static size_t prime_count = 0;

/* Build / enlarge the prime cache so that it contains at least n_needed primes. */
static void
recompute_primes(idx_t n_needed)
{
    /* Upper bound for the n-th prime: p_n < n * (log n + log log n) for n ≥ 6.
       For our usage n ≤ 10000, the following simple bound is more than enough. */
    idx_t limit = (n_needed < 6) ? 15 : n_needed * 20 + 100;

    /* Sieve of Eratosthenes */
    bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime)
        return;                     /* memory error – unlikely in tests */

    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes and build cache */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    seq_t *new_cache = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!new_cache) {
        free(is_prime);
        return;                     /* memory error – unlikely */
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            new_cache[pos++] = (seq_t)i;

    /* Replace old cache */
    free(prime_cache);
    prime_cache = new_cache;
    prime_count = cnt;

    free(is_prime);
}

/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* unused, offset is 1 */

    if (prime_count < n)
        recompute_primes(n);

    if (n == 1)
        return (seq_t)2;

    /* nth prime is stored at index n-1 */
    seq_t p = prime_cache[n - 1];
    return (p + (seq_t)3) / (seq_t)2;
}
