/*
 * A282569  Number of compositions (ordered partitions) of n into
 *          multiplicatively perfect numbers (A007422).
 * https://oeis.org/A282569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 37, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A282569 – number of ordered partitions of n into
 *  multiplicatively perfect numbers (numbers with exactly four divisors).
 *  Recurrence:  a(0)=1
 *               a(n)=∑_{m∈MP, m≤n} a(n-m),   MP={1} ∪ {p*q | p<q primes}
 *                 ∪ {p^3 | p prime}.
 *  The program builds the set MP up to the requested n and
 *  then computes the recurrence by dynamic programming.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* cache for the values a(n) */
static seq_t *dp = NULL;          /* dp[i] = a(i)                     */
static idx_t dp_len = 0;          /* allocated length of dp array      */
static idx_t max_n_computed = 0;  /* highest n for which dp[0..n] is correct */

/* list of multiplicatively perfect numbers up to current limit */
static idx_t *mp = NULL;          /* array of MP numbers               */
static size_t mp_len = 0;
static size_t mp_cap = 0;

/* prime table up to current limit */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* Ensure that we have primes up to at least 'limit' */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    free(primes);
    build_primes_upto(limit, &primes, &prime_cnt);
    prime_limit = limit;
}

/* Rebuild the list of multiplicatively perfect numbers up to 'limit'.
 * The set is {1} ∪ {p^3, p prime, p^3 ≤ limit} ∪
 *                           {p*q, p<q primes, p*q ≤ limit}.
 */
static void ensure_mp_list(idx_t limit)
{
    if (mp_cap < 16) {          /* initial capacity */
        mp_cap = 16;
        mp = (idx_t *)realloc(mp, mp_cap * sizeof(idx_t));
    }
    mp_len = 0;
    mp[mp_len++] = 1;            /* 1 is multiplicatively perfect */

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];

        /* add p^3 if it fits */
        if ((seq_t)p * p * p <= limit) {
            if (mp_len >= mp_cap) {
                mp_cap = mp_cap * 2 + 16;
                mp = (idx_t *)realloc(mp, mp_cap * sizeof(idx_t));
            }
            mp[mp_len++] = p * p * p;
        }

        /* add p*q for all q>p */
        for (size_t j = i + 1; j < prime_cnt; ++j) {
            idx_t q = primes[j];
            if (p > limit / q)          /* avoid overflow / too large */
                break;
            idx_t prod = p * q;
            if (prod > limit)
                break;
            if (mp_len >= mp_cap) {
                mp_cap = mp_cap * 2 + 16;
                mp = (idx_t *)realloc(mp, mp_cap * sizeof(idx_t));
            }
            mp[mp_len++] = prod;
        }
    }
}

/* Allocate or grow the DP array to cover indices up to 'limit'. */
static void ensure_dp(idx_t limit)
{
    if (dp_len == 0) {                     /* first allocation */
        dp_len = limit + 1;
        dp = (seq_t *)malloc(dp_len * sizeof(seq_t));
        for (idx_t i = 0; i < dp_len; ++i)
            dp[i] = 0;
        dp[0] = 1;
        max_n_computed = 0;
    } else if (limit >= dp_len) {          /* extend the array */
        dp = (seq_t *)realloc(dp, (limit + 1) * sizeof(seq_t));
        for (idx_t i = dp_len; i <= limit; ++i)
            dp[i] = 0;
        dp_len = limit + 1;
    }

    /* Compute new values only in the uncovered range. */
    idx_t start = max_n_computed + 1;
    if (start > limit)
        return;                            /* nothing new to compute */

    /* Ensure primes and MP list are up to 'limit' */
    ensure_primes_upto(limit);
    ensure_mp_list(limit);

    for (idx_t i = start; i <= limit; ++i) {
        seq_t sum = 0;
        for (size_t k = 0; k < mp_len; ++k) {
            idx_t m = mp[k];
            if (m > i)
                continue;                  /* skip larger terms */
            sum += dp[i - m];
        }
        dp[i] = sum;
    }
    max_n_computed = limit;
}

/* Public interface required by the harness. */
seq_t a(idx_t n)
{
    ensure_dp(n);
    return dp[n];
}
