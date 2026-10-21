/*
 * A249611  Integers that reach the (15, 8) cycle described in A234534, after
 *          iterations of numerator(sigma(n)/n) = A017665(n).
 * https://oeis.org/A249611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of sequence A249611.
   For each integer m >= 2 it iteratively applies
   f(m) = sigma(m)/gcd(sigma(m),m)  where sigma is the sum of divisors.
   Numbers that eventually reach the 2‑cycle {8,15} are collected.
   The function builds the sequence lazily and returns the nth term. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_uint64.h"

/* --- utility functions -------------------------------------------------- */

/* -------- prime generation (sieve up to 1,000,000) ----------------------- */
#define PRIME_LIMIT 1000000
#define PRIME_ESTIMATE 80000   /* number of primes up to PRIME_LIMIT */

static uint32_t _primes[PRIME_ESTIMATE];
static size_t _prime_cnt = 0;
static bool _primes_ready = false;

static void init_primes(void)
{
    if (_primes_ready) return;
    bool *is_comp = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_comp) exit(EXIT_FAILURE);
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (_prime_cnt < PRIME_ESTIMATE) _primes[_prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    _primes_ready = true;
}

/* --------- integer factorisation --------------------------------------- */
static void factor_u64(uint64_t n, uint32_t *pr, uint32_t *ex, size_t *cnt)
{
    init_primes();
    uint64_t tmp = n;
    *cnt = 0;
    for (size_t i = 0; i < _prime_cnt && (uint64_t)_primes[i] * _primes[i] <= tmp; ++i) {
        if (tmp % _primes[i] == 0) {
            uint32_t e = 0;
            while (tmp % _primes[i] == 0) {
                tmp /= _primes[i];
                ++e;
            }
            pr[(*cnt)] = _primes[i];
            ex[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (tmp > 1) {
        pr[(*cnt)] = (uint32_t)tmp;   /* tmp fits in 32‑bit for our bounds */
        ex[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* --------- sum of divisors --------------------------------------------- */
static uint64_t sigma_uint64(uint64_t n)
{
    uint32_t fac_pr[32], fac_ex[32];
    size_t fac_cnt;
    factor_u64(n, fac_pr, fac_ex, &fac_cnt);
    __uint128_t res = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = fac_pr[i];
        uint64_t e = fac_ex[i];
        __uint128_t pk = 1;                /* p^(e+1) */
        for (uint64_t k = 0; k <= e; ++k) pk *= p;
        __uint128_t term = (pk - 1) / (p - 1);
        res *= term;
    }
    return (uint64_t)res;   /* assumes result fits in 64‑bit */
}

/* --------- next term of the iterated function ---------------------------- */
static uint64_t next_uint64(uint64_t n)
{
    uint64_t s = sigma_uint64(n);
    uint64_t g = gcd_uint64(s, n);
    return s / g;
}

/* --------- does an integer eventually fall into {8,15} ? --------------- */
static bool reaches_cycle(uint64_t start)
{
    if (start == 8 || start == 15) return true;
    uint64_t cur = start;
    /* visited array for cycle detection */
    uint64_t visited[256];
    size_t  vis_cnt = 0;
    while (true) {
        cur = next_uint64(cur);
        if (cur == 8 || cur == 15) return true;
        /* check repetition */
        for (size_t i = 0; i < vis_cnt; ++i) {
            if (visited[i] == cur) return false;
        }
        if (vis_cnt < sizeof(visited) / sizeof(visited[0]))
            visited[vis_cnt++] = cur;
        else
            return false;           /* safety guard */
    }
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0 */
    static seq_t *terms = NULL;
    static size_t   terms_size = 0;
    static size_t   terms_cap  = 0;
    static uint64_t next_candidate = 2;   /* first integer to test */

    /* grow cache until we have at least n terms */
    while (terms_size < (size_t)n) {
        uint64_t m = next_candidate++;
        if (reaches_cycle(m)) {
            /* store m as seq_t */
            if (terms == NULL || terms_size == terms_cap) {
                size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
                seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_size++] = (seq_t)m;
        }
    }
    return terms[n - 1];
}
