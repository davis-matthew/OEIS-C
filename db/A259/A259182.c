/*
 * A259182  a(n) = prime(n) if n prime otherwise 1.
 * https://oeis.org/A259182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) if n is prime, otherwise 1
 * Range:     u8 n <= 58, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   is_idx_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_idx_prime.h"

/* -------------  Prime storage  ------------- */
static size_t         g_limit      = 0;      /* current sieve limit           */
static size_t         g_prime_cnt  = 0;      /* number of primes stored       */
static uint64_t      *g_primes     = NULL;   /* array of primes (1‑based)     */

/* Build enough primes so that the n‑th prime is stored in g_primes[n‑1]. */
static void build_primes_upto_n(idx_t n)
{
    if ((size_t)n <= g_prime_cnt) return;   /* already enough */

    size_t new_limit = g_limit ? g_limit * 2 : 100;
    while (1) {
        uint8_t *comp = (uint8_t *)calloc(new_limit + 1, 1);
        if (!comp) exit(EXIT_FAILURE);

        /* Sieve of Eratosthenes */
        for (size_t i = 2; i * i <= new_limit; ++i) {
            if (!comp[i]) {
                for (size_t j = i * i; j <= new_limit; j += i)
                    comp[j] = 1;
            }
        }

        /* Collect primes */
        size_t est = new_limit / 5 + 10;
        uint64_t *pr = (uint64_t *)malloc(est * sizeof(uint64_t));
        if (!pr) { free(comp); exit(EXIT_FAILURE); }

        size_t cnt = 0;
        for (size_t i = 2; i <= new_limit; ++i) {
            if (!comp[i]) {
                if (cnt == est) {          /* grow buffer if needed */
                    est *= 2;
                    uint64_t *tmp = (uint64_t *)realloc(pr, est * sizeof(uint64_t));
                    if (!tmp) { free(comp); free(pr); exit(EXIT_FAILURE); }
                    pr = tmp;
                }
                pr[cnt++] = (uint64_t)i;
            }
        }
        free(comp);

        free(g_primes);
        g_primes = pr;
        g_limit  = new_limit;
        g_prime_cnt = cnt;

        if ((size_t)n <= g_prime_cnt) break;  /* enough primes obtained */
        new_limit *= 2;                      /* enlarge and try again */
    }
}

/* -------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */

    if (!is_idx_prime(n)) return (seq_t)1; /* composite index */

    build_primes_upto_n(n);              /* ensure nth prime is available */
    return (seq_t)g_primes[n - 1];       /* primes stored 0‑based */
}
