/*
 * A330558  a(n) = number of primes p <= 2*n+1 with Delta(p) == 0 mod 4, where
 *          Delta(p) = nextprime(p) - p.
 * https://oeis.org/A330558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2518, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count primes p ≤ 2*n+1 such that (nextprime(p) − p) ≡ 0 (mod 4) */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Minimal upper bound for the sieve: 2*n+1 + 200 guarantees the
       next prime after the largest prime ≤ 2*n+1 is included. */
    idx_t limit   = 2*n + 1;
    idx_t sieve_limit = limit + 200;

    /* Allocate an array of booleans for the sieve */
    bool *is_prime = (bool*)malloc((sieve_limit + 1) * sizeof(bool));
    if (!is_prime) return 0;          /* fallback for malloc failure */

    /* Initialize */
    for (idx_t i = 0; i <= sieve_limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    /* Sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= sieve_limit; ++p)
        if (is_prime[p])
            for (idx_t q = p * p; q <= sieve_limit; q += p)
                is_prime[q] = false;

    /* Count the primes that satisfy the condition */
    seq_t count = 0;
    for (idx_t p = 2; p <= limit; ++p)
        if (is_prime[p]) {
            /* Find the next prime after p */
            idx_t q = p + 1;
            while (q <= sieve_limit && !is_prime[q]) ++q;
            if (q > sieve_limit) continue; /* should not happen */
            idx_t delta = q - p;
            if ((delta & 3) == 0) ++count;  /* delta % 4 == 0 */
        }

    free(is_prime);
    return count;
}
