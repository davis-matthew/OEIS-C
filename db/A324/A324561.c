/*
 * A324561  Numbers with at least one prime index equal to 0, 1, or 4 modulo
 *          5.
 * https://oeis.org/A324561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A324561: a(n) = n-th positive integer having at least one prime factor
 * whose prime index ≡ 0, 1, 4 (mod 5).  The function builds a linear-spf
 * sieve and caches qualifying numbers; the cache is replenished automatically
 * as larger numbers are requested.  */
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------- */
/*  Sieve and lookup tables                                            */
/* ------------------------------------------------------------------- */
static idx_t spf_limit  = 0;      /* current sieve upper bound       */
static idx_t *spf_tbl   = NULL;   /* smallest prime factor table    */
static idx_t *prime_idx = NULL;   /* index of each prime (1‑based)   */
static idx_t *primes    = NULL;   /* list of primes up to spf_limit  */
static size_t primes_cnt = 0;      /* number of primes in primes     */

/* ------------------------------------------------------------------- */
/*  Cache for qualifying numbers                                        */
/* ------------------------------------------------------------------- */
static seq_t *cache_vals  = NULL;  /* cached numbers satisfying the criterion */
static size_t cache_len   = 0;     /* number of cached numbers             */
static size_t cache_cap   = 0;     /* current capacity of cache_vals       */
static idx_t next_candidate = 2;   /* next integer to examine             */

/* ------------------------------------------------------------------- */
/*  Build a linear sieve up to `limit`.  Allocates spf_tbl, prime_idx
 *  and primes[ ].  The tables are rebuilt from scratch each time.
 * ------------------------------------------------------------------- */
static void build_sieve(idx_t limit)
{
    free(spf_tbl);
    free(prime_idx);
    free(primes);

    spf_tbl   = (idx_t*)calloc((size_t)(limit + 1), sizeof(idx_t));
    prime_idx = (idx_t*)calloc((size_t)(limit + 1), sizeof(idx_t));
    if (!spf_tbl || !prime_idx) exit(1);

    size_t approx = (size_t)(limit / 10 + 10);
    primes = (idx_t*)malloc(approx * sizeof(idx_t));
    if (!primes) exit(1);
    primes_cnt = 0;

    /* Linear sieve for smallest prime factor */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf_tbl[i] == 0) {                    /* i is prime */
            spf_tbl[i] = i;
            if (primes_cnt == approx) {           /* grow buffer */
                approx <<= 1;
                primes = (idx_t*)realloc(primes, approx * sizeof(idx_t));
                if (!primes) exit(1);
            }
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            if (p > spf_tbl[i]) break;
            idx_t v = i * p;
            if (v > limit) break;
            spf_tbl[v] = p;
        }
    }

    /* Build 1‑based prime index table */
    for (size_t i = 0; i < primes_cnt; ++i) {
        prime_idx[primes[i]] = (idx_t)(i + 1);
    }

    spf_limit = limit;
}

/* ------------------------------------------------------------------- */
/*  Return true if x has at least one prime factor whose prime index
 *  is 0, 1, or 4 (mod 5).                                            */
/* ------------------------------------------------------------------- */
static bool has_good_prime_index(idx_t x)
{
    idx_t y = x;
    while (y > 1) {
        idx_t p   = spf_tbl[y];
        idx_t idx = prime_idx[p];
        int mod   = (int)(idx % 5);
        if (mod == 0 || mod == 1 || mod == 4) return true;
        /* Skip all powers of this prime */
        while (y % p == 0) y /= p;
    }
    return false;
}

/* ------------------------------------------------------------------- */
/*  Ensure that at least `n` qualifying terms have been cached.  The
 *  sieve is expanded when the current limit is exhausted.
 * ------------------------------------------------------------------- */
static void ensure_terms(idx_t n)
{
    if (spf_limit == 0) build_sieve(200000);   /* initial limit */

    while (cache_len < n) {
        /* Scan numbers up to current sieve limit */
        for (; next_candidate <= spf_limit && cache_len < n; ++next_candidate) {
            if (has_good_prime_index(next_candidate)) {
                if (cache_len == cache_cap) {          /* grow cache */
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    cache_vals = (seq_t*)realloc(cache_vals,
                                                 cache_cap * sizeof(seq_t));
                    if (!cache_vals) exit(1);
                }
                cache_vals[cache_len++] = (seq_t)next_candidate;
            }
        }
        if (cache_len < n) {                          /* increase sieve */
            idx_t new_limit = spf_limit * 2;
            build_sieve(new_limit);
        }
    }
}

/* ------------------------------------------------------------------- */
/*  Public interface: a(n)                                              */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */
    ensure_terms(n);
    return cache_vals[n - 1];
}
