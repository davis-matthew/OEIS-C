/*
 * A383439  a(n) = (5*n)!/((n!)^2*(3*n + 1)!).
 * https://oeis.org/A383439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*n)! / ( (n!)^2 * (3*n+1)! )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Legendre's formula: exponent of prime p in m! */
static idx_t fact_prime_exp(idx_t m, idx_t p) {
    idx_t exp = 0;
    while (m) {
        m /= p;
        exp += m;
    }
    return exp;
}

seq_t a(idx_t n) {
    /* Special case a(0) */
    if (n == 0) return (seq_t)1;

    /* ---- 1. Generate all primes up to 5*n via sieve ---- */
    idx_t limit = 5 * n;
    /* allocate (limit+1) booleans, zero-initialised to mark composites */
    bool *is_comp = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_comp) return (seq_t)0;            /* allocation failure */

    /* rough estimate of prime count; will grow if needed */
    size_t primes_est = (size_t)(limit / 3) + 10;
    idx_t *primes = (idx_t *)malloc(primes_est * sizeof(idx_t));
    if (!primes) {
        free(is_comp);
        return (seq_t)0;
    }

    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            /* store prime */
            if (primes_cnt == primes_est) {
                primes_est *= 2;
                primes = (idx_t *)realloc(primes, primes_est * sizeof(idx_t));
            }
            primes[primes_cnt++] = i;

            /* mark multiples of i */
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);         /* sieve finished */

    /* ---- 2. Compute result via prime exponents ---- */
    seq_t result = (seq_t)1;

    for (size_t id = 0; id < primes_cnt; ++id) {
        idx_t p = primes[id];

        /* exponent in numerator (5*n)! */
        idx_t exp = fact_prime_exp(5 * n, p);

        /* subtract exponent in (3*n+1)! */
        exp -= fact_prime_exp(3 * n + 1, p);

        /* subtract twice exponent in n! */
        exp -= 2 * fact_prime_exp(n, p);

        /* multiply by p^exp */
        for (idx_t i = 0; i < exp; ++i)
            result *= (seq_t)p;
    }

    free(primes);
    return result;
}
