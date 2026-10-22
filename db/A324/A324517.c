/*
 * A324517  Numbers > 1 where the maximum prime index equals the number of
 *          prime factors minus the number of distinct prime factors.
 * https://oeis.org/A324517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A324517: numbers >1 where maximum prime index equals
   number of prime factors minus number of distinct prime factors.
   Implemented via incremental search with trial division. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define SIEVE_LIMIT 2000U          /* primes up to 2000 are enough for the needed indices   */
#define MAX_SMALL_PRIMES 300       /* number of primes ≤ 2000 */

/* ----------------------------------------------------------------- */
/*  Small prime sieve – gives prime list and index of each prime ≤ LIMIT  */
static bool sieve_built = false;
static uint32_t primes[MAX_SMALL_PRIMES];
static size_t     prime_cnt = 0;
static uint16_t   prime_index_map[SIEVE_LIMIT + 1];  /* 0 → not a prime / out of range   */

static void build_small_primes(void)
{
    if (sieve_built) return;

    bool is_prime[SIEVE_LIMIT + 1];
    for (unsigned i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (unsigned i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (unsigned j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = false;

    size_t idx = 0;
    for (unsigned i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            primes[idx++] = i;
            prime_index_map[i] = (uint16_t)(idx);   /* 1‑based index */
        }
    }
    prime_cnt = idx;
    sieve_built = true;
}

/* ----------------------------------------------------------------- */
/*  Test whether a given n satisfies the defining equality           */
static bool candidate_ok(uint64_t n)
{
    uint64_t temp = n;
    uint64_t Omega   = 0;   /* total number of prime factors (with multiplicity) */
    uint64_t omega   = 0;   /* number of distinct prime factors             */
    uint64_t maxIdx  = 0;   /* maximum prime index (1‑based)                */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > temp) break;
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            Omega += exp;
            ++omega;
            uint16_t idx = prime_index_map[p];
            if (idx > maxIdx) maxIdx = idx;
        }
    }

    /* Remaining factor, if any (must be prime) */
    if (temp > 1) {
        if (temp > SIEVE_LIMIT) return false;          /* index too large → fail   */
        uint16_t idx = prime_index_map[(uint32_t)temp];
        if (idx == 0) return false;                    /* not a prime – impossible */
        Omega += 1;
        ++omega;
        if (idx > maxIdx) maxIdx = idx;
    }

    return maxIdx == (Omega - omega);
}

/* ----------------------------------------------------------------- */
/*  Dynamic array to store found sequence values                       */
static seq_t *seq_vals = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

static void seq_grow(void)
{
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq_vals = tmp;
    seq_cap  = new_cap;
}

/* ----------------------------------------------------------------- */
/*  Incremental sequence generator                                    */
static uint64_t next_candidate = 4;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* no definition for offset 0 */

    build_small_primes();

    while (seq_len < (size_t)n) {
        while (!candidate_ok(next_candidate)) ++next_candidate;

        if (seq_len == seq_cap) seq_grow();
        seq_vals[seq_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return seq_vals[n - 1];
}
