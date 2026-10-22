/*
 * A398671  a(n) is the number of positive integers <= n with no prime factors
 *          of form 4*k+1.
 * https://oeis.org/A398671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 552, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of positive integers ≤ n having no prime factor of the form 4*k+1
   Computed by sieving up to n and counting the numbers that are not a multiple
   of any prime ≡ 1 (mod 4). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t cur_limit = 0;       /* largest n that has been computed */
    static seq_t *counts   = NULL;    /* cumulative counts: counts[i] = a(i) */
    static unsigned char *forbidden = NULL;   /* forbidden[i] = 1 if i has a 4k+1 prime factor */

    if (n < 1) return (seq_t)0;  /* offset is 1, but a(0) is unused */

    if (n > cur_limit) {
        /* Allocate new arrays of size n+1 (index 0..n) */
        size_t sz = (size_t)(n + 1);
        seq_t *new_counts = (seq_t *)malloc(sz * sizeof(seq_t));
        unsigned char *new_forbidden = (unsigned char *)calloc(sz, 1); /* zeros */
        if (!new_counts || !new_forbidden) {
            free(new_counts);
            free(new_forbidden);
            return (seq_t)0;
        }

        /* Sieve of Eratosthenes to find primes up to n */
        unsigned char *is_prime = (unsigned char *)malloc(sz);
        if (!is_prime) {
            free(new_counts);
            free(new_forbidden);
            return (seq_t)0;
        }
        /* Initially assume all numbers are prime, except 0 and 1 */
        for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
        if (sz > 0) { is_prime[0] = 0; is_prime[1] = 0; }

        for (idx_t i = 2; i * i <= n; ++i) {
            if (is_prime[(size_t)i]) {
                for (idx_t j = i * i; j <= n; j += i) {
                    is_prime[(size_t)j] = 0;
                }
            }
        }

        /* Mark numbers that are divisible by a prime ≡ 1 (mod 4) */
        for (idx_t p = 2; p <= n; ++p) {
            if (!is_prime[(size_t)p]) continue;
            if ((p & 3) == 1) {            /* p ≡ 1 (mod 4) */
                for (idx_t j = p; j <= n; j += p)
                    new_forbidden[(size_t)j] = 1;
            }
        }
        free(is_prime);

        /* Build cumulative counts */
        new_counts[0] = 0;
        for (idx_t i = 1; i <= n; ++i) {
            new_counts[(size_t)i] = new_counts[(size_t)(i - 1)]
                                     + (new_forbidden[(size_t)i] ? 0 : 1);
        }

        /* Replace old cache with the new one */
        free(counts);
        free(forbidden);
        counts = new_counts;
        forbidden = new_forbidden;
        cur_limit = n;
    }

    return counts[(size_t)n];
}
