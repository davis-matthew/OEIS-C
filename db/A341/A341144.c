/*
 * A341144  Number of partitions of n into 7 distinct prime powers (including
 *          1).
 * https://oeis.org/A341144
 *
 * Offset:    30    (a(30) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as a sum of 7 distinct prime
 *            powers (including 1)
 * Range:     u8 n <= 67, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* --- Static data used for memoisation ----------------------------------- */
static idx_t max_stored = 0;                 /* highest n that has been built   */
static seq_t *dp[8] = {NULL};                /* dp[k][s] – subsets of size k   */
static uint64_t *prime_powers = NULL;        /* all distinct prime powers ≤ n   */
static size_t pp_len = 0;                    /* number of elements in list      */

/* Rebuild all tables to cover targets up to new_max. */
static void rebuild(idx_t new_max)
{
    size_t k;

    /* free old tables */
    for (k = 0; k <= 7; ++k) {
        if (dp[k]) free(dp[k]);
        dp[k] = NULL;
    }
    if (prime_powers) free(prime_powers);
    prime_powers = NULL;
    pp_len = 0;

    /* allocate fresh DP tables (size new_max+1, zeroed) */
    for (k = 0; k <= 7; ++k) {
        dp[k] = (seq_t *)calloc((size_t)new_max + 1, sizeof(seq_t));
        if (!dp[k]) return;   /* allocation failure – leave NULL tables */
    }

    /* generate primes up to new_max */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(new_max, &primes, &primes_cnt);

    /* count how many prime powers are needed (including 1) */
    size_t total = 1; /* for the value 1 */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t val = p;
        while (val <= new_max) {
            ++total;
            if (val > new_max / p) break;
            val *= p;
        }
    }

    /* allocate array to store prime powers */
    prime_powers = (uint64_t *)malloc(total * sizeof(uint64_t));
    if (!prime_powers) {
        free(primes);
        return;   /* allocation failure */
    }

    /* store the prime powers, starting with 1 */
    size_t pos = 0;
    prime_powers[pos++] = 1ULL;          /* 1 is a prime power for this sequence */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t val = p;
        while (val <= new_max) {
            prime_powers[pos++] = val;
            if (val > new_max / p) break;
            val *= p;
        }
    }
    pp_len = total;
    free(primes);

    /* DP base case: exactly one way to choose 0 numbers that sum to 0 */
    dp[0][0] = 1;

    /* DP transition: iterate once over each available prime power to enforce distinctness */
    for (size_t id = 0; id < pp_len; ++id) {
        uint64_t v = prime_powers[id];
        for (int kk = 7; kk >= 1; --kk) {
            for (idx_t s = new_max; s >= (idx_t)v; --s) {
                dp[kk][s] += dp[kk - 1][s - (idx_t)v];
            }
        }
    }

    max_stored = new_max;
}

/* ------------------------------------------------------------------------- */
/*  Public function required by the kata.                                   */
seq_t a(idx_t n)
{
    if (n < 30) return (seq_t)0;            /* behaviour for n<30 is irrelevant */

    if (n > max_stored) rebuild(n);
    if (!dp[7]) return (seq_t)0;            /* allocation failure guard */

    return dp[7][n];
}
