/*
 * A304733  The largest prime factor of the n-th term of EKG-sequence: a(n) =
 *          A006530(A064413(n)).
 * https://oeis.org/A304733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 122, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = largest prime factor of the n‑th term of the EKG sequence.
   The EKG sequence is defined by:
        a(1)=1, a(2)=2, and for n>2,
        a(n) = smallest unused positive integer that shares a common factor >1
               with a(n-1).
   This function generates terms of the EKG sequence up to the requested
   index using a sieve‑based prime list and a next‑multiple table to find
   the next term efficiently.  The largest prime factor of each term is
   stored in a cache and returned.  The implementation works for any of
   the seq_t types accepted by the build system. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* External helper prototypes required by the build interface.        */
/* ------------------------------------------------------------------ */
#include "../utils/largest_prime_factor.h"   /* uint64_t largest_prime_factor(uint64_t m) */

/* ------------------------------------------------------------------ */
/* Static data for caching EKG terms, largest prime factors and prime  */
/* tables.  All arrays grow as needed.                                */
/* ------------------------------------------------------------------ */
static seq_t  *ekg_cache  = NULL;   /* EKG sequence terms   */
static seq_t  *lpf_cache  = NULL;   /* Largest prime factor of each term */
static size_t  ekg_len    = 0;
static size_t  ekg_cap    = 0;

static uint8_t *used      = NULL;   /* used[i] = 1 if number i already used   */
static size_t  used_cap   = 0;

/* Prime table ----------------------------------- */
#define PRIME_LIMIT 2000000   /* should be enough for n ≤ 10000 */
static int    *prime_list  = NULL;
static size_t  prime_cnt   = 0;
static uint64_t *next_mult = NULL;     /* for primes ≤ PRIME_LIMIT */

/* Flag indicating that the tables have been initialized. */
static bool initialized = false;

/* ------------------------------------------------------------------ */
/* Helper: ensure that the prime tables exist.                        */
/* ------------------------------------------------------------------ */
static void init_prime_table(void)
{
    if (prime_cnt > 0) return;      /* already built */

    size_t sieve_size = PRIME_LIMIT + 1;
    bool *sieve = (bool*)malloc(sieve_size);
    if (!sieve) exit(EXIT_FAILURE);
    for (size_t i = 0; i < sieve_size; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;
        }
    }

    /* Count primes */
    prime_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) if (sieve[i]) ++prime_cnt;

    prime_list = (int*)malloc(prime_cnt * sizeof(int));
    if (!prime_list) exit(EXIT_FAILURE);

    for (size_t i = 2, j = 0; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) prime_list[j++] = (int)i;

    /* next_mult array */
    next_mult = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!next_mult) exit(EXIT_FAILURE);
    for (size_t i = 0; i < prime_cnt; ++i) next_mult[i] = (uint64_t)prime_list[i];

    free(sieve);
}

/* ------------------------------------------------------------------ */
/* Factor a 64‑bit integer into distinct prime factors.                */
/* The result is written into `factors` (≤ 64 elements) and the count  */
/* is returned in *out_cnt.  The factors are sorted increasingly.      */
/* ------------------------------------------------------------------ */
static void factor_uint64(uint64_t n, uint64_t *factors, size_t *out_cnt)
{
    size_t cnt = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= n; ++i) {
        uint64_t p = (uint64_t)prime_list[i];
        if (n % p == 0) {
            factors[cnt++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) factors[cnt++] = n;
    *out_cnt = cnt;
}

/* ------------------------------------------------------------------ */
/* Ensure the `used` array can index at least idx.                     */
/* ------------------------------------------------------------------ */
static void ensure_used_cap(size_t idx)
{
    if (idx < used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 1;
    while (new_cap <= idx) new_cap *= 2;
    uint8_t *tmp = (uint8_t*)realloc(used, new_cap);
    if (!tmp) exit(EXIT_FAILURE);
    /* initialise new part to 0 */
    for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = 0;
    used = tmp;
    used_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Ensure the cache contains at least `need` terms of the EKG        */
/* sequence and their largest prime factors.                           */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t need)
{
    if (!initialized) {
        /* initial allocation */
        ekg_cap = 1024;
        ekg_cache = (seq_t*)malloc(ekg_cap * sizeof(seq_t));
        lpf_cache = (seq_t*)malloc(ekg_cap * sizeof(seq_t));
        if (!ekg_cache || !lpf_cache) exit(EXIT_FAILURE);

        ekg_cache[0] = (seq_t)1;
        ekg_cache[1] = (seq_t)2;
        lpf_cache[0] = (seq_t)1;
        lpf_cache[1] = (seq_t)largest_prime_factor(2ULL);
        ekg_len = 2;

        used_cap = 4;    /* allow indices 0..3 */
        used = (uint8_t*)calloc(used_cap, 1);
        if (!used) exit(EXIT_FAILURE);
        used[1] = 1;
        used[2] = 1;

        init_prime_table();
        initialized = true;
    }

    if (ekg_len >= (size_t)need) return;

    /* Grow cache arrays if needed */
    if (ekg_len + (size_t)(need - ekg_len) > ekg_cap) {
        size_t new_cap = ekg_cap;
        while (new_cap < ekg_len + (size_t)(need - ekg_len)) new_cap *= 2;
        ekg_cache = (seq_t*)realloc(ekg_cache, new_cap * sizeof(seq_t));
        lpf_cache = (seq_t*)realloc(lpf_cache, new_cap * sizeof(seq_t));
        if (!ekg_cache || !lpf_cache) exit(EXIT_FAILURE);
        ekg_cap = new_cap;
    }

    /* Temporary buffer for factorisation */
    uint64_t prime_factors[64];
    size_t fp_cnt;

    /* Generate terms until we reach `need` */
    while (ekg_len < (size_t)need) {
        seq_t last = ekg_cache[ekg_len - 1];
        uint64_t last_u = (uint64_t)last;   /* we only need last up to 64‑bit */

        /* Factor last into distinct primes */
        factor_uint64(last_u, prime_factors, &fp_cnt);

        /* Find candidate: smallest unused multiple of any prime factor */
        uint64_t best = UINT64_MAX;
        for (size_t i = 0; i < fp_cnt; ++i) {
            uint64_t p = prime_factors[i];
            uint64_t cand;

            if (p <= PRIME_LIMIT) {
                /* use precomputed next multiple */
                size_t idx = 0;
                while (prime_list[idx] != (int)p) ++idx;
                cand = next_mult[idx];
                /* advance until unused */
                while (true) {
                    ensure_used_cap(cand);
                    if (!used[cand]) break;
                    cand += p;
                }
            } else {
                /* Handle large prime p on the fly */
                cand = p;
                while (true) {
                    ensure_used_cap(cand);
                    if (!used[cand]) break;
                    cand += p;
                }
            }
            if (cand < best) best = cand;
        }

        /* Record the new term */
        ekg_cache[ekg_len] = (seq_t)best;
        uint64_t lpf = largest_prime_factor(best);
        lpf_cache[ekg_len] = (seq_t)lpf;
        ekg_len++;

        /* Mark as used */
        ensure_used_cap(best);
        used[best] = 1;

        /* Update next_mult for the prime factors of the chosen number */
        factor_uint64(best, prime_factors, &fp_cnt);
        for (size_t i = 0; i < fp_cnt; ++i) {
            uint64_t p = prime_factors[i];
            if (p <= PRIME_LIMIT) {
                size_t idx = 0;
                while (prime_list[idx] != (int)p) ++idx;
                if (next_mult[idx] == best) {
                    next_mult[idx] = best + p;
                    while (true) {
                        ensure_used_cap(next_mult[idx]);
                        if (!used[next_mult[idx]]) break;
                        next_mult[idx] += p;
                    }
                }
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system.                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache((size_t)n);
    return lpf_cache[n - 1];
}
