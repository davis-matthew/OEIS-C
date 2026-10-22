/*
 * A324521  Numbers > 1 where the maximum prime index is less than or equal to
 *          the number of prime factors counted with multiplicity.
 * https://oeis.org/A324521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 1664, u32 n <= 9422, u64 n <= 9422,
 *            u128 n <= 9422, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A324521: a(n) = the n-th smallest integer m > 1 such that
   the maximum prime index of m is ≤ Ω(m), where Ω(m) counts prime
   factors with multiplicity. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* --------------------------------------------------------------------- */
/*  Parameters and static data                                           */
/* --------------------------------------------------------------------- */
#define SIEVE_LIMIT 2000000U   /* should be large enough for n up to ~10 000 */

static bool sieve_done = false;
static uint32_t *prime_index = NULL;   /* prime_index[p] = index of prime p (1‑based) */
static uint32_t *primes = NULL;        /* list of primes up to SIEVE_LIMIT */
static size_t primes_sz = 0;

static seq_t *seq = NULL;              /* array of found terms          */
static size_t seq_sz = 0;              /* number of elements stored    */
static size_t seq_cap = 0;             /* capacity of seq array        */
static idx_t next_candidate = 2;       /* next integer to test         */

/* --------------------------------------------------------------------- */
/*  Simple Sieve of Eratosthenes to build `prime_index` and `primes`.   */
/* --------------------------------------------------------------------- */
static void init_sieve(void)
{
    if (sieve_done)
        return;

    bool *sieve = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!sieve)
        return;                      /* failure unlikely in test env */

    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        sieve[i] = true;

    for (idx_t i = 2; (idx_t)i * (idx_t)i <= SIEVE_LIMIT; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = false;

    prime_index = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    primes = (uint32_t *)calloc(SIEVE_LIMIT / 10, sizeof(uint32_t)); /* rough upper bound */

    if (!prime_index || !primes) {
        free(sieve);
        return;                      /* memory error handling is minimal */
    }

    size_t cap = SIEVE_LIMIT / 10;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (sieve[i]) {
            if (primes_sz == cap) {
                cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, cap * sizeof(uint32_t));
                if (!tmp) {
                    free(sieve);
                    free(prime_index);
                    free(primes);
                    return;          /* realloc failure */
                }
                primes = tmp;
            }
            primes[primes_sz] = (uint32_t)i;
            prime_index[i] = (uint32_t)(primes_sz + 1);       /* 1‑based index */
            ++primes_sz;
        }
    }

    free(sieve);
    sieve_done = true;
}

/* --------------------------------------------------------------------- */
/*  Test whether a 64‑bit integer satisfies the defining property.      */
/* --------------------------------------------------------------------- */
static bool satisfies_uint64(uint64_t v)
{
    if (v <= 1)
        return false;

    uint64_t tmp = v;
    unsigned int omega = 0;          /* Ω(v) */
    unsigned int max_idx = 0;        /* maximum prime index */

    for (size_t i = 0; i < primes_sz && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p != 0)
            continue;
        unsigned int idx = prime_index[p];
        if (idx > max_idx)
            max_idx = idx;
        unsigned int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        omega += exp;
    }

    if (tmp > 1) {                  /* remaining prime factor */
        if ((uint64_t)tmp <= SIEVE_LIMIT) {
            unsigned int idx = prime_index[(uint32_t)tmp];
            if (idx > max_idx)
                max_idx = idx;
        } else {
            /* In our usage (n ≤ 10 000) this path should not be taken. */
        }
        ++omega;
    }

    return max_idx <= omega;
}

/* --------------------------------------------------------------------- */
/*  Ensure the term array has room for `needed` elements.                */
/* --------------------------------------------------------------------- */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    if (!seq) {
        seq = seq_alloc(new_cap);
    } else {
        seq = seq_realloc(seq, seq_cap, new_cap);
    }
    seq_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  External sequence function.                                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    init_sieve();

    if ((size_t)n > seq_sz) {
        while (seq_sz < (size_t)n) {
            if (satisfies_uint64((uint64_t)next_candidate)) {
                ensure_capacity(seq_sz + 1);
                seq[seq_sz++] = (seq_t)next_candidate;
            }
            ++next_candidate;
        }
    }
    return seq[n - 1];
}
