/*
 * A390361  Numbers k such that the first sums of 1-prepended prime indices of
 *          k are all distinct. Positions of strict rows in A390308.
 * https://oeis.org/A390361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 183, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A390361: numbers k such that when the prime indices
 * (with multiplicity) of k are prepended by 1, all sums of
 * consecutive elements are distinct.
 *
 * Observation: The condition is equivalent to
 * * the exponent of 2 in k is at most 1 (i.e., k is not divisible
 *   by 4),
 * * and for every odd prime p dividing k, the exponent of p is at
 *   most 2 (i.e., k is not divisible by p³).
 *
 * Thus a number is good iff it is square‑free except possibly for
 * squares of odd primes.
 *
 * The function below generates the sequence by incremental search.
 * It uses a sieve for smallest prime factors to factor each candidate.
 * Cache is stored to allow arbitrary‑order calls.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global state for the sieve and cache */
static uint32_t *spf = NULL;          /* smallest prime factor table   */
static idx_t sieve_limit = 0;         /* current sieve upper bound     */
static idx_t last_checked = 0;        /* last integer examined         */

/* Cache for computed sequence values */
static seq_t *seq_cache = NULL;       /* array of a(1)...a(k)          */
static size_t seq_cap = 0;            /* capacity of seq_cache         */
static size_t seq_len = 0;            /* number of stored terms        */

static void build_sieve(idx_t limit)
{
    if (spf) free((void *)spf);
    spf = (uint32_t *)calloc(limit + 1U, sizeof(uint32_t));

    for (idx_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {          /* i is prime */
            spf[i] = (uint32_t)i;
            if (i <= limit / i) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i) {
                    if (!spf[(size_t)j]) spf[(size_t)j] = (uint32_t)i;
                }
            }
        }
    }
}

static void ensure_globals(void)
{
    if (!sieve_limit) {                    /* first call only */
        sieve_limit = 100;                 /* initial small limit */
        build_sieve(sieve_limit);
        seq_cap = 16;
        seq_cache = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    }
}

/* Test whether k satisfies the property */
static bool is_good(idx_t k)
{
    if (k == 1) return true;

    idx_t tmp = k;
    while (tmp > 1) {
        uint32_t p = spf[(size_t)tmp];
        int count = 0;
        while ((tmp % p) == 0) {
            tmp /= p;
            ++count;
        }
        if (p == 2) {
            if (count > 1) return false;   /* 2^2 or higher not allowed */
        } else {
            if (count > 2) return false;   /* odd prime cube or higher not allowed */
        }
    }
    return true;
}

static void append_seq(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = val;
}

seq_t a(idx_t n)
{
    ensure_globals();

    /* Generate terms until we have at least n */
    while (seq_len < n) {
        /* Grow sieve if necessary */
        if (last_checked >= sieve_limit) {
            sieve_limit *= 2;
            if (sieve_limit < 100) sieve_limit = 100;
            build_sieve(sieve_limit);
        }

        /* Check candidates from last_checked+1 up to sieve_limit */
        for (idx_t m = last_checked + 1; m <= sieve_limit; ++m) {
            if (is_good(m)) append_seq((seq_t)m);
        }
        last_checked = sieve_limit;
    }

    return seq_cache[n - 1];
}
