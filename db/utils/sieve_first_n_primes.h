/* sieve_first_n_primes: the first n primes.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_SIEVE_FIRST_N_PRIMES_H
#define OEISC_UTILS_SIEVE_FIRST_N_PRIMES_H
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --- prime generation using a single sieve ------------------------------------------------- */
static void sieve_first_n_primes(idx_t n, uint32_t **primes_out, size_t *cnt_out)
{
    /* Upper bound for the n-th prime (for n >= 6): p_n < n(ln n + ln ln n) */
    idx_t limit;
    if (n < 6) {
        limit = 15;
    } else {
        double dn = (double)n;
        double upper = dn * (log(dn) + log(log(dn))) + 3.0 * dn; /* generous safety */
        limit = (idx_t)upper + 10;
    }

    /* Allocate space for collecting primes */
    size_t est_capacity = (size_t)(n + 16);
    uint32_t *primes = (uint32_t *)malloc(est_capacity * sizeof(uint32_t));
    size_t primes_cnt = 0;

    /* Sieve of Eratosthenes */
    unsigned char *is_prime = (unsigned char *)malloc((limit + 1) * sizeof(unsigned char));
    memset(is_prime, 1, (limit + 1));
    is_prime[0] = is_prime[1] = 0;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= (uint64_t)limit; j += i)
                is_prime[j] = 0;
        }
    }
    for (uint32_t i = 2; i <= (uint32_t)limit && primes_cnt < (size_t)n; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == est_capacity) {
                est_capacity <<= 1;
                primes = (uint32_t *)realloc(primes, est_capacity * sizeof(uint32_t));
            }
            primes[primes_cnt++] = i;
        }
    }

    free(is_prime);
    /* In the extremely unlikely event we did not obtain enough primes, increase limit and retry */
    while (primes_cnt < (size_t)n) {
        limit <<= 1;                               /* double the limit */
        free(primes);
        est_capacity = (size_t)(n + 16);
        primes = (uint32_t *)malloc(est_capacity * sizeof(uint32_t));
        primes_cnt = 0;

        is_prime = (unsigned char *)malloc((limit + 1) * sizeof(unsigned char));
        memset(is_prime, 1, (limit + 1));
        is_prime[0] = is_prime[1] = 0;
        for (uint64_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (uint64_t j = i * i; j <= (uint64_t)limit; j += i)
                    is_prime[j] = 0;
            }
        }
        for (uint32_t i = 2; i <= (uint32_t)limit && primes_cnt < (size_t)n; ++i) {
            if (is_prime[i]) {
                if (primes_cnt == est_capacity) {
                    est_capacity <<= 1;
                    primes = (uint32_t *)realloc(primes, est_capacity * sizeof(uint32_t));
                }
                primes[primes_cnt++] = i;
            }
        }
        free(is_prime);
    }

    *primes_out = primes;
    *cnt_out = primes_cnt;
}

#endif
