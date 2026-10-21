/*
 * A243887  (p^2 - 3)/2 for odd primes p.
 * https://oeis.org/A243887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_n^2 - 3)/2 where p_n is the n-th odd prime
 * Range:     u8 n <= 7, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n<1 */

    /* Lazy prime sieve: store all primes found so far. */
    static uint32_t *primes   = NULL;
    static size_t   prime_cap = 0;
    static size_t   prime_cnt = 0;
    static uint32_t  candidate = 3;   /* next odd candidate to test */

    /* Initialise the array with the first prime 2. */
    if (prime_cnt == 0) {
        prime_cap = 16;
        primes = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        if (!primes) return (seq_t)0;          /* allocation failed */
        primes[0] = 2;
        prime_cnt = 1;
    }

    /* Generate primes until we have at least (n+1) primes
       (because the n-th odd prime is prime number n+1 overall). */
    while (prime_cnt < (size_t)(n + 1)) {
        /* Find the next prime. */
        for (;;) {
            if ((candidate & 1U) == 0) {  /* even numbers are not prime */
                candidate += 1;
                continue;
            }

            uint32_t c = candidate;
            int is_prime = 1;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint32_t p = primes[i];
                if ((uint64_t)p * p > (uint64_t)c) break;
                if (c % p == 0) { is_prime = 0; break; }
            }

            if (is_prime) {
                /* enlarge the array if necessary */
                if (prime_cnt >= prime_cap) {
                    size_t new_cap = prime_cap << 1;
                    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                    if (!tmp) return (seq_t)0;  /* allocation failed */
                    primes = tmp;
                    prime_cap = new_cap;
                }
                primes[prime_cnt++] = c;
                candidate = c + 2;  /* next odd number */
                break;
            } else {
                candidate += 2;
            }
        }
    }

    /* Retrieve the n-th odd prime (indexing from 1). */
    uint32_t p = primes[n];   /* primes[1] = 3, primes[2] = 5, ... */

    /* Compute (p^2 - 3)/2 using seq_t arithmetic. */
    seq_t val = ((seq_t)p * (seq_t)p - (seq_t)3) / (seq_t)2;
    return val;
}
