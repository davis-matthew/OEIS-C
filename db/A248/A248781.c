/*
 * A248781  Number of integers k^4 that divide n!
 * https://oeis.org/A248781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{p≤n} (⌊v_p(n!)/4⌋ + 1) where v_p(n!) = ∑_{k≥1} ⌊n /
 *            p^k⌋ is the exponent of prime p in n!
 * Range:     u8 n <= 35, u16 n <= 80, u32 n <= 194, u64 n <= 471,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n < 2: there are no primes ≤ n. */
    if (n < 2)
        return 1;

    /* ---------- 1. Sieve of Eratosthenes to find all primes ≤ n ---------- */
    /* Allocate an array for primality flags.  Index 0..n. */
    unsigned char *is_prime = (unsigned char*)malloc((size_t)(n + 1));
    if (!is_prime)
        return 0; /* allocation failure – unlikely with small n */

    for (idx_t i = 0; i <= n; ++i)
        is_prime[i] = 1;              /* assume prime initially          */
    is_prime[0] = 0;
    is_prime[1] = 0;

    for (idx_t i = 2; (idx_t)(i * i) <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count how many primes we will collect. */
    size_t prime_count = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i])
            ++prime_count;

    /* Allocate an array to hold the primes. */
    idx_t *primes = NULL;
    if (prime_count > 0) {
        primes = (idx_t*)malloc(prime_count * sizeof(idx_t));
        if (!primes) {
            free(is_prime);
            return 0; /* allocation failure – unlikely with small n */
        }
        size_t c = 0;
        for (idx_t i = 2; i <= n; ++i)
            if (is_prime[i])
                primes[c++] = i;
        prime_count = c; /* should equal original count */
    }

    free(is_prime); /* primality array no longer needed */

    /* ---------- 2. Compute the product over primes ---------- */
    seq_t result = 1;

    for (size_t pi = 0; pi < prime_count; ++pi) {
        idx_t p = primes[pi];

        /* Compute v_p(n!) = floor(n/p) + floor(n/p^2) + ... */
        idx_t v = 0;
        idx_t power = p;
        while (power <= n) {
            v += n / power;
            /* Prevent overflow of power * p when p^k exceeds n. */
            if (power > n / p)
                break;
            power *= p;
        }

        seq_t factor = (seq_t)(v / 4 + 1); /* ⌊v/4⌋ + 1 */
        result *= factor;
    }

    free(primes); /* free prime list if allocated */

    return result;
}
