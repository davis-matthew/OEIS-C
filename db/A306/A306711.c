/*
 * A306711  Numbers k such that gcd(k, phi(k)) <> gcd(k, psi(k)).
 * https://oeis.org/A306711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer k such that * gcd(k, φ(k)) ≠
 *            gcd(k, ψ(k)), where φ is Euler’s totient * and ψ is Dedekind’s
 *            psi function. The function searches * successive k and caches
 *            the found terms; no explicit * formula is known
 * Range:     u8 n <= 34, u16 n <= 9375, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>     /* malloc, realloc, abort */
#include <stdint.h>     /* uint64_t */
#include <stddef.h>     /* size_t */
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_arr = NULL; /* primes up to PRIMELIMIT */
static size_t   prime_cnt = 0;
static const idx_t PRIMELIMIT = 200000;   /* enough for n≈10000 */

/* Initialise the prime table once. */
static void init_primes(void)
{
    if (!prime_arr) {
        build_primes_upto(PRIMELIMIT, &prime_arr, &prime_cnt);
    }
}

seq_t a(idx_t n)
{
    /* Cache of discovered terms. */
    static seq_t *terms = NULL;
    static size_t cap   = 0;   /* current capacity of 'terms' */
    static size_t len   = 0;   /* number of terms stored */
    static idx_t last_k = 1;   /* last k examined */

    if (n == 0) return 0; /* undefined for offset 1, but safe treat */

    init_primes();

    /* Generate terms until we have at least n of them. */
    while (len < (size_t)n) {
        last_k++;                    /* next candidate k */
        seq_t k = (seq_t)last_k;     /* convert to seq_t for operations */
        uint64_t temp = last_k;      /* copy for trial division */
        seq_t phi = k;
        seq_t psi = k;

        /* Factor k using the prime table. */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_arr[i];
            if (p * p > (uint64_t)temp) break;
            if (temp % p == 0) {
                /* distinct prime factor found */
                phi = phi / (seq_t)p * ((seq_t)p - 1);
                psi = psi / (seq_t)p * ((seq_t)p + 1);
                while (temp % p == 0) temp /= p;
            }
        }
        if (temp > 1) { /* remaining prime factor > sqrt(original k) */
            uint64_t p = temp;
            phi = phi / (seq_t)p * ((seq_t)p - 1);
            psi = psi / (seq_t)p * ((seq_t)p + 1);
        }

        /* Check the defining condition. */
        if (gcd(k, phi) != gcd(k, psi)) {
            /* Grow the cache if necessary. */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failure */
                terms = tmp;
                cap   = new_cap;
            }
            terms[len++] = k;
        }
    }

    return terms[(size_t)n - 1];
}
