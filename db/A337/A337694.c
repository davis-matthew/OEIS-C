/*
 * A337694  Numbers with no two relatively prime prime indices.
 * https://oeis.org/A337694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer containing no pair of relatively prime
 *            prime indices. The prime index of a prime p is the position m
 *            such that prime(m) = p (starting from 1). For an integer, its
 *            prime‑index multiset consists of indices for each prime factor,
 *            counted with multiplicity. The number is in th...
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd32.h"

static const idx_t SIEVE_LIMIT = 2000000;   /* upper bound for sieving */

static idx_t *spf;                  /* smallest prime factor table   */
static uint32_t *prime_idx;         /* 1‑based index of each prime  */
static uint32_t *primes;            /* list of primes                */
static size_t prime_cnt;

/* cache for the sequence */
static seq_t *seq_cache;
static size_t seq_len;
static size_t seq_cap;
static idx_t next_test;

/* initialize the sieve and supporting tables */
static void init_sieve(void)
{
    spf = (idx_t *)calloc(SIEVE_LIMIT + 1, sizeof(idx_t));
    prime_idx = (uint32_t *)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));

    /* smallest prime factor sieve */
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i)
        spf[i] = i;
    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                if (spf[j] == j)
                    spf[j] = i;
        }
    }

    /* primes list and index mapping (1‑based) */
    primes = (uint32_t *)malloc((SIEVE_LIMIT / 10) * sizeof(uint32_t));
    prime_cnt = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == i) {
            prime_idx[i] = (uint32_t)(prime_cnt + 1);  /* 1‑based index */
            primes[prime_cnt++] = (uint32_t)i;
        } else
            prime_idx[i] = 0;
    }

    /* sequence cache initialisation */
    seq_cache = NULL;
    seq_len   = 0;
    seq_cap   = 0;
    next_test = 1;
}

/* ensure that seq_cache can hold 'needed' elements */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* true if m has no pair of relatively prime prime indices */
static bool good_candidate(idx_t m)
{
    if (m == 1)
        return true;

    uint32_t indices[32];
    size_t count = 0;

    idx_t tmp = m;
    while (tmp > 1) {
        uint32_t p   = (uint32_t)spf[tmp];
        uint32_t idx = prime_idx[p];

        unsigned e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }

        for (unsigned t = 0; t < e; ++t) {
            for (size_t i = 0; i < count; ++i) {
                if (gcd32(idx, indices[i]) == 1)
                    return false;
            }
            indices[count++] = idx;
            /* numbers <= SIEVE_LIMIT cannot have more than ~21
               indices with multiplicity, so count < 32 always */
        }
    }
    return true;
}

/* generate sequence terms until at least n terms exist */
static void ensure_n_terms(idx_t n)
{
    static bool first = true;
    if (first) {
        init_sieve();
        first = false;
    }

    while (seq_len < n) {
        bool ok = good_candidate(next_test);
        if (ok) {
            ensure_seq_cap(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_test;
        }
        ++next_test;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    ensure_n_terms(n);
    return seq_cache[n - 1];
}
