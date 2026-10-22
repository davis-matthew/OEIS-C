/*
 * A395515  Sum of the distinct prime divisors of C(4*n, n).
 * https://oeis.org/A395515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime divisors of binomial(4*n, n)
 * Range:     u8 n <= 14, u16 n <= 300, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Compute a(n) = sopf(C(4*n, n)) */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* upper bound for primes needed in the binomial coefficient */
    idx_t limit = 4 * n;

    /* --- Sieve of Eratosthenes to generate primes ≤ limit --- */
    bool *is_composite = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_composite) {
        /* on allocation failure just return 0 (should not happen for the
           intended input ranges) */
        return 0;
    }

    uint64_t *primes = NULL;
    size_t primes_cap = 0;
    size_t primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            /* store new prime */
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                if (!tmp) {
                    free(is_composite);
                    free(primes);
                    return 0; /* allocation failure */
                }
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = (uint64_t)i;

            /* mark multiples as composite */
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    is_composite[j] = true;
                }
            }
        }
    }
    free(is_composite);

    /* --- Compute the sum of distinct prime factors --- */
    seq_t sum = 0;

    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];

        /* exponent of p in (4*n)! */
        unsigned long long vp_4n = 0;
        unsigned long long t = (unsigned long long)4 * n;
        while (t) {
            t /= p;
            vp_4n += t;
        }

        /* exponent of p in n! */
        unsigned long long vp_n = 0;
        t = (unsigned long long)n;
        while (t) {
            t /= p;
            vp_n += t;
        }

        /* exponent of p in (3*n)! */
        unsigned long long vp_3n = 0;
        t = (unsigned long long)3 * n;
        while (t) {
            t /= p;
            vp_3n += t;
        }

        /* if exponent in the binomial coefficient is positive,
           include the prime once in the sum */
        if (vp_4n > vp_n + vp_3n) {
            sum += (seq_t)p;
        }
    }

    free(primes);
    return sum;
}
