/*
 * A296924  Primes p such that Legendre(-6,p) = 0 or 1.
 * https://oeis.org/A296924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that Legendre(-6, p) is 0 or 1. For
 *            p = 2 or 3 Legendre is 0; otherwise Legendre(-6,p) is computed
 *            by the Jacobi symbol, which for odd primes equals the Legendre
 *            symbol. The implementation sieves primes up to a dynamically
 *            increasing limit and keeps a list of primes sa...
 * Range:     u8 n <= 29, u16 n <= 3256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"

static seq_t *selected_primes = NULL;     /* holds the qualifying primes */
static size_t selected_len  = 0;           /* number of primes stored   */
static uint64_t sieve_limit = 0;          /* current sieve maximum     */

/* recompute the sieve and the list of qualifying primes up to 'limit' */
static void recompute_all(uint64_t limit)
{
    /* allocate and build the primality sieve */
    uint8_t *is_prime = (uint8_t *)calloc(limit + 1, 1);
    if (!is_prime) return;                 /* allocation failed */

    is_prime[0] = 0;
    is_prime[1] = 0;
    for (uint64_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }

    /* free any old prime list and start fresh */
    free(selected_primes);
    selected_primes = NULL;
    selected_len   = 0;
    size_t cap = 1024;
    selected_primes = (seq_t *)malloc(cap * sizeof(seq_t));

    /* scan primes and test the Legendre condition */
    for (uint64_t p = 2; p <= limit; ++p)
        if (is_prime[p]) {
            bool add = false;
            if (p == 2 || p == 3)      /* Legendre = 0 */
                add = true;
            else {
                uint64_t a = (2 * p - 6) % p;  /* (-6) mod p */
                int leg = jacobi_symbol(a, p); /* Legendre for prime p */
                if (leg == 1) add = true;        /* Legendre = 1 */
            }
            if (add) {
                if (selected_len == cap) {
                    cap *= 2;
                    selected_primes = (seq_t *)realloc(selected_primes,
                                                     cap * sizeof(seq_t));
                }
                selected_primes[selected_len++] = (seq_t)p;
            }
        }

    free(is_prime);
    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset <1 */

    static bool first = true;
    if (first) {
        recompute_all(300000);             /* initial limit for n≈10000 */
        first = false;
    }

    while (selected_len < (size_t)n) {
        uint64_t new_limit = sieve_limit * 2;
        if (new_limit <= sieve_limit)     /* overflow guard */
            new_limit = sieve_limit + 100000;
        recompute_all(new_limit);
    }

    return selected_primes[n - 1];
}
