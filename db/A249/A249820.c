/*
 * A249820  a(1) = 0 and for n > 1: a(n) = A249810(n) - A078898(n) =
 *          A078898(A003961(n)) - A078898(n).
 * https://oeis.org/A249820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A078898(A003961(n)) - A078898(n) * A003961(n) = product
 *            of next prime to each prime factor of n * (multiplying exponents
 *            unchanged). * A078898(n) = rank of n in the block of integers
 *            having the same * smallest prime factor (index starting from 1).
 *            * a(1) = 0
 * Range:     i8 n <= 95, i16 n <= 3071, i32 n <= 3071, i64 n <= 3071,
 *            i128 n <= 3071, big any n
 * Helpers:   build_spf, factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/build_spf.h"

#define LIMIT 2000000ULL  /* enclose all values needed for the tested range */

static idx_t *spf_arr = NULL;          /* smallest prime factor */
static uint32_t *rank_arr = NULL;      /* A078898(n) for n <= LIMIT */
static bool initialized = false;

/* ---------------------------------------------------------------------- */
/*                         Sieve and rank precomputation                    */
/* ---------------------------------------------------------------------- */
static void init(void)
{
    if (initialized) return;

    /* build spf array */
    build_spf(LIMIT, &spf_arr);

    /* allocate rank array */
    rank_arr = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!rank_arr) { /* fallback: abort if allocation fails */
        exit(1);
    }

    /* temporary counter per prime (indexed by the prime itself) */
    uint32_t *cnt = (uint32_t*)calloc((LIMIT + 1), sizeof(uint32_t));
    if (!cnt) { free(rank_arr); exit(1); }

    rank_arr[0] = rank_arr[1] = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        uint32_t p = (uint32_t)spf_arr[i];
        ++cnt[p];
        rank_arr[i] = cnt[p];
    }

    free(cnt);
    initialized = true;
}

/* ---------------------------------------------------------------------- */
/*                         helper functions                               */
/* ---------------------------------------------------------------------- */

/* Return A078898(n)  (rank in its block) for n up to LIMIT. */
static seq_t rank_of(idx_t n)
{
    if (n <= LIMIT) return (seq_t)rank_arr[(uint32_t)n];
    /* fallback for n > LIMIT  (rare for the tested range) */
    static bool warned = false;
    if (!warned) { warned = true; }
    size_t fcnt;
    uint64_t primes[64], exps[64];
    factor_u64(n, primes, exps, &fcnt);
    if (fcnt == 0) return (seq_t)0;
    uint64_t p = primes[0];
    seq_t cnt = 0;
    for (idx_t k = 2; k <= n; ++k) {
        uint64_t spf_k;
        if (k <= LIMIT) spf_k = spf_arr[k];
        else {
            size_t fc2;
            uint64_t primes2[64], exps2[64];
            factor_u64(k, primes2, exps2, &fc2);
            spf_k = primes2[0];
        }
        if (spf_k == p) ++cnt;
    }
    return cnt;
}

/* Compute A003961(n) for n <= LIMIT and return as idx_t. */
static idx_t map_next_prime_factors_idx(idx_t n)
{
    if (n <= 1) return (idx_t)1;
    size_t fcnt;
    uint64_t primes[64], exps[64];
    factor_u64(n, primes, exps, &fcnt);
    idx_t result = 1;
    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t nextp = next_prime(primes[i]);
        uint32_t e = (uint32_t)exps[i];
        idx_t powv = 1;
        for (uint32_t j = 0; j < e; ++j) {
            powv *= nextp;
        }
        result *= powv;
    }
    return result;
}

/* ---------------------------------------------------------------------- */
/*                         public sequence function                       */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;   /* a(1) = 0 by definition */

    if (!initialized) init();

    seq_t col_n      = rank_of(n);
    idx_t mapped_val = map_next_prime_factors_idx(n);
    seq_t col_mapped = rank_of(mapped_val);

    return col_mapped - col_n;    /* subtraction done in seq_t */
}
