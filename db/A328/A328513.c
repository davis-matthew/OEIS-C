/*
 * A328513  Connected squarefree numbers.
 * https://oeis.org/A328513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth connected squarefree number
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   gcd_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: squarefree numbers that are connected as per the
   graph on prime indices having an edge when the indices share a
   common divisor > 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/gcd_u32.h"

/* global sieve data ----------------------------------------------------- */
static uint8_t *sieve = NULL;
static uint32_t sieve_limit = 0;
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static uint32_t *prime_to_idx = NULL;  /* prime_to_idx[p] = index (1‑based) */

/* build sieve up to at least 'limit' ----------------------------------- */
static void build_sieve_up_to(uint32_t limit) {
    if (limit <= sieve_limit) return;

    /* free previous data */
    free(sieve);
    free(primes);
    free(prime_to_idx);

    sieve_limit = limit;
    sieve = (uint8_t*)calloc(limit + 1, 1);
    if (!sieve) abort();

    /* simple sieve of Eratosthenes */
    for (uint32_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (uint32_t j = i * i; j <= limit; j += i) sieve[j] = 1;
        }
    }

    /* count primes and allocate arrays */
    primes_cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i) if (!sieve[i]) ++primes_cnt;

    primes = (uint32_t*)malloc(primes_cnt * sizeof(uint32_t));
    prime_to_idx = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    if (!primes || !prime_to_idx) abort();

    /* populate primes and index table */
    size_t idx = 1;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            primes[idx - 1] = i;
            prime_to_idx[i] = (uint32_t)idx;   /* 1‑based index */
            ++idx;
        }
    }
}

/* check connectivity of the index list ----------------------------------- */
static bool is_connected(const uint32_t *idxs, size_t cnt) {
    if (cnt <= 1) return true;
    bool *vis = (bool*)calloc(cnt, 1);
    if (!vis) abort();
    size_t stack[64];
    size_t sp = 0;
    vis[0] = true;
    stack[sp++] = 0;

    while (sp) {
        uint32_t u = stack[--sp];
        for (size_t v = 0; v < cnt; ++v) {
            if (!vis[v] && gcd_u32(idxs[u], idxs[v]) > 1) {
                vis[v] = true;
                stack[sp++] = v;
            }
        }
    }

    bool ok = true;
    for (size_t i = 0; i < cnt; ++i) if (!vis[i]) { ok = false; break; }
    free(vis);
    return ok;
}

/* ------------------------------------------------------------------------- */
/* Public function that returns the nth connected squarefree number          */
seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialize sieve once with a generous limit */
    static bool initialized = false;
    if (!initialized) {
        build_sieve_up_to(2000000);  /* should cover all needed primes for n<=10000 */
        initialized = true;
    }

    /* static variable holding the next candidate to test */
    static uint64_t candidate = 1;

    /* generate terms until we reach the requested index */
    while (cache_size < n) {
        uint64_t cand = candidate;
        /* factor cand and collect prime indices */
        uint64_t tmp = cand;
        bool squarefree = true;
        uint32_t idxs[32];
        size_t idx_cnt = 0;

        for (size_t pi = 0; pi < primes_cnt && (uint64_t)primes[pi] * primes[pi] <= tmp; ++pi) {
            uint32_t p = primes[pi];
            if (tmp % p == 0) {
                int exp = 0;
                do { tmp /= p; ++exp; } while (tmp % p == 0);
                if (exp > 1) { squarefree = false; break; }
                if (prime_to_idx[p] == 0) build_sieve_up_to(p);
                idxs[idx_cnt++] = prime_to_idx[p];
            }
        }

        if (squarefree && tmp > 1) {
            uint32_t p = (uint32_t)tmp;
            if (prime_to_idx[p] == 0) build_sieve_up_to(p);
            idxs[idx_cnt++] = prime_to_idx[p];
        }

        if (squarefree && is_connected(idxs, idx_cnt)) {
            cache = (seq_t*)realloc(cache, (cache_size + 1) * sizeof(seq_t));
            cache[cache_size++] = (seq_t)cand;
        }

        ++candidate;
    }

    return cache[n - 1];
}
