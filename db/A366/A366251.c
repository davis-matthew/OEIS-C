/*
 * A366251  Numbers with a coprime divisor shift.
 * https://oeis.org/A366251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A366251: Numbers with a coprime divisor shift */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Core function required by the host */
seq_t a(idx_t n);

/* ========================================================================== */
/*  Helper data structures and constants                                   */
/* ========================================================================== */

#define MAX_PRIME_ARRAY 32

/* Cache for computed terms */
static seq_t *term_cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* Smallest prime factor sieve */
static uint64_t *spf = NULL;        /* spf[i] = smallest prime factor of i */
static size_t spf_limit = 0;        /* current size of spf array */

/* Auxiliary array used to mark seen residues while testing a prime divisor */
static int *flag = NULL;           /* flag[r] == stamp iff r has been seen */
static int stamp = 1;              /* increasing stamp for each prime */

/* Global candidate generator */
static uint64_t next_candidate = 1;   /* start from 1 */

/* ========================================================================== */
/*  Sieve helpers                                                             */
/* ========================================================================== */

static void init_sieve(size_t limit)
{
    spf = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
    for (size_t i = 0; i <= limit; ++i) spf[i] = i;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {
            for (size_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    flag = (int*)malloc((limit + 1) * sizeof(int));
    for (size_t i = 0; i <= limit; ++i) flag[i] = 0;
    spf_limit = limit;
}

static void ensure_sieve(size_t min_limit)
{
    if (min_limit <= spf_limit) return;

    size_t new_limit = spf_limit ? spf_limit : 4;
    while (new_limit < min_limit) new_limit <<= 1;

    uint64_t *new_spf = (uint64_t*)malloc((new_limit + 1) * sizeof(uint64_t));
    for (size_t i = 0; i <= new_limit; ++i) new_spf[i] = i;
    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (new_spf[i] == i) {
            for (size_t j = i * i; j <= new_limit; j += i)
                if (new_spf[j] == j) new_spf[j] = i;
        }
    }
    free(spf);
    spf = new_spf;

    int *new_flag = (int*)malloc((new_limit + 1) * sizeof(int));
    for (size_t i = 0; i <= new_limit; ++i) new_flag[i] = 0;
    free(flag);
    flag = new_flag;

    spf_limit = new_limit;
}

/* ========================================================================== */
/*  Residue generation and prime‑factor test                                 */
/* ========================================================================== */

static void dfs_div(int idx, uint64_t cur, const uint64_t *pr,
                    const uint8_t *exps, int cnt, uint64_t mod, int *seen)
{
    if (idx == cnt) {
        uint64_t r = cur % mod;          /* 0 <= r < mod */
        if (r != 0 && flag[(int)r] != stamp) {
            flag[(int)r] = stamp;
            (*seen)++;
        }
        return;
    }

    uint64_t val = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        dfs_div(idx + 1, (cur * val) % mod,
                pr, exps, cnt, mod, seen);
        val = (val * pr[idx]) % mod;
    }
}

static bool candidate_ok(uint64_t k)
{
    uint64_t x = k;
    uint64_t primes[MAX_PRIME_ARRAY];
    uint8_t  exps[MAX_PRIME_ARRAY];
    int pcnt = 0;

    /* Factorise k with the global SPF sieve */
    while (x > 1) {
        uint64_t p = spf[x];
        uint8_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        primes[pcnt] = p;
        exps[pcnt]  = e;
        ++pcnt;
    }

    /* For each prime divisor check that divisors of the remaining part
       do NOT cover all residues modulo that prime. */
    for (int i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];

        /* Build list of primes/exponents of k' = k / p^e */
        uint64_t other_pr[MAX_PRIME_ARRAY];
        uint8_t  other_exp[MAX_PRIME_ARRAY];
        int ocnt = 0;
        for (int j = 0; j < pcnt; ++j) {
            if (j == i) continue;
            other_pr[ocnt] = primes[j];
            other_exp[ocnt] = exps[j];
            ++ocnt;
        }

        ++stamp;               /* new stamp for this prime */
        int seen_cnt = 0;

        if (ocnt == 0) {        /* k' == 1 */
            int r = 1 % (int)p;
            if (flag[r] != stamp) {
                flag[r] = stamp;
                ++seen_cnt;
            }
        } else {
            dfs_div(0, 1 % p, other_pr, other_exp, ocnt, p, &seen_cnt);
        }

        if (seen_cnt == (int)p - 1)  /* full set of residues found */
            return false;
    }

    return true;
}

/* ========================================================================== */
/*  Main function: returns the nth term of A366251                         */
/* ========================================================================== */

seq_t a(idx_t n)
{
    static bool initialized = false;

    if (!initialized) {
        /* Initial sieve size large enough for typical n <= 10000 */
        ensure_sieve(2000000);
        cache_cap = 16;
        term_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_candidate = 1;
        initialized = true;
    }

    while (cache_len < n) {
        if (next_candidate > spf_limit)
            ensure_sieve(next_candidate * 2);

        uint64_t k = next_candidate;

        if ((k & 1) && candidate_ok(k)) {   /* only odd numbers can satisfy */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *newc = (seq_t*)realloc(term_cache,
                                              new_cap * sizeof(seq_t));
                term_cache = newc;
                cache_cap = new_cap;
            }
            term_cache[cache_len++] = (seq_t)k;
        }

        ++next_candidate;
    }

    return term_cache[n - 1];
}
