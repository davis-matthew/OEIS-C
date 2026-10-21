/*
 * A291205  Numbers n such that psi(n) = phi(m) has no solution for any m
 *          (psi(n) = A001615(n) and phi(m) = A000010(m)).
 * https://oeis.org/A291205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 808, u32 n <= 8226, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n‑th integer n such that Dedekind ψ(n) is not a
   totient number.  The sequence is defined by
   a(n) = the n‑th integer for which ψ(n) ≠ φ(m) for any m.  */

/*  We pre‑compute all primes ≤ LIMIT, all totient values ≤ LIMIT
 *  and a flag array isTotient[ψ] to test whether ψ is a totient.
 *  Then we generate the terms on demand, caching the results.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* global constants and buffers */
static const uint32_t LIMIT = 20000000u;          /* upper bound for n and totients */
static uint32_t *primes = NULL;                  /* prime list */
static size_t primes_cnt = 0;
static bool *isTotient = NULL;                  /* flag for totient values */
static bool initialized = false;

static seq_t *cache = NULL;                     /* cache of sequence terms */
static size_t cache_len = 0, cache_cap = 0;
static uint64_t next_n = 2;                     /* next integer to test */

/* --------------------------------------------------------------------- */
/* build_data  –  initialise primes, totient flag array, and other data
 * Called only once by a().  */
static void build_data(void)
{
    if (initialized) return;

    /* ---- sieve for primes up to LIMIT  ---- */
    bool *is_prime = (bool*)calloc((size_t)(LIMIT + 1u), sizeof(bool));
    size_t *primes_tmp = (size_t*)malloc((LIMIT / 10u + 1000u) * sizeof(size_t));
    primes_cnt = 0;

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_prime[i]) {
            if (primes_cnt == 0) {
                /* initial allocation, already allocated above */
            }
            primes_tmp[primes_cnt++] = i;
            if (i <= LIMIT / i) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    is_prime[j] = true;
            }
        }
    }
    primes = (uint32_t*)malloc(primes_cnt * sizeof(uint32_t));
    for (size_t k = 0; k < primes_cnt; ++k) primes[k] = (uint32_t)primes_tmp[k];
    free(primes_tmp);
    free(is_prime);

    /* ---- compute φ for all m ≤ LIMIT  ---- */
    uint32_t *phi = (uint32_t*)calloc((size_t)(LIMIT + 1u), sizeof(uint32_t));
    for (uint32_t i = 0; i <= LIMIT; ++i) phi[i] = i;

    for (uint32_t p = 2; p <= LIMIT; ++p) {
        if (phi[p] == p) {                     /* p is prime */
            for (uint32_t j = p; j <= LIMIT; j += p) {
                phi[j] -= phi[j] / p;
            }
        }
    }

    /* ---- build totient flag array  ---- */
    isTotient = (bool*)calloc((size_t)(LIMIT + 1u), sizeof(bool));
    for (uint32_t m = 2; m <= LIMIT; ++m) {
        uint32_t v = phi[m];
        if (v <= LIMIT) isTotient[v] = true;
    }
    isTotient[1] = true;          /* φ(1) = 1 */

    free(phi);
    initialized = true;
}

/* --------------------------------------------------------------------- */
/* psi_of  –  compute Dedekind ψ(n) using the prime list                  */
static seq_t psi_of(uint64_t n)
{
    seq_t psi = (seq_t)n;
    uint64_t t = n;

    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        uint64_t p = primes[idx];
        if (p * p > t) break;
        if (t % p == 0) {
            /* distinct prime factor found */
            psi = psi * (seq_t)(p + 1) / (seq_t)p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) {
        /* remaining prime factor */
        psi = psi * (seq_t)(t + 1) / (seq_t)t;
    }
    return psi;
}

/* --------------------------------------------------------------------- */
/* a  –  sequence term requested by index n (1‑based)                     */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below the offset is irrelevant */

    build_data();

    /* generate terms until we have at least n entries */
    while (cache_len < (size_t)n) {
        uint64_t m = next_n++;          /* next integer to test */
        seq_t psi = psi_of(m);

        bool is_t = false;
        if (psi <= (seq_t)LIMIT && isTotient[(size_t)psi])
            is_t = true;

        if (!is_t) {                    /* m is part of the sequence */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)m;
        }
    }
    return cache[n - 1];
}
