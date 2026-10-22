/*
 * A340966  Number of ways to write n as an ordered sum of 10 primes.
 * https://oeis.org/A340966
 *
 * Offset:    20    (a(20) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 36, u32 n <= 159, u64 n <= 3507,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, build_primes_upto, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A340966 – Number of ways to write n as an ordered sum of 10 primes.
    Formula: a(n) = coefficient of xⁿ in (∑_{p prime} xᵖ )¹⁰           */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* ---------- static cache ---------- */
static idx_t      max_n   = 0;          /* largest n processed so far          */
static seq_t     *dp[11]  = {0};        /* dp[k][s] : k = 1..10, s = 0..max_n */
static uint64_t *primes  = NULL;        /* primes up to max_n                  */
static size_t    prime_cnt = 0;         /* number of primes                     */

/* ---------- helper: rebuild tables for n ≥ current max_n ---------- */
static void ensure_computed(idx_t n)
{
    if (n <= max_n) return;                    /* nothing to do */

    /* free old DP tables */
    for (int k = 1; k <= 10; ++k)
        if (dp[k]) free_seq_array(dp[k]);

    /* free old prime list */
    if (primes) free(primes);

    /* build new tables up to n */
    max_n = n;

    /* allocate DP arrays – zero‑initialised */
    for (int k = 1; k <= 10; ++k)
        dp[k] = alloc_seq_array((size_t)max_n + 1);

    /* get all primes ≤ n */
    build_primes_upto((idx_t)max_n, &primes, &prime_cnt);

    /* DP[1] – one way to sum a single prime equal to itself */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p <= max_n) dp[1][p] = 1;
    }

    /* DP for k = 2 .. 10 */
    for (int k = 2; k <= 10; ++k) {
        seq_t *prev = dp[k - 1];
        seq_t *curr = dp[k];
        /* curr is already zeroed by alloc_seq_array */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > (uint64_t)max_n) break;
            size_t start = (size_t)p;
            for (size_t s = start; s <= (size_t)max_n; ++s) {
                curr[s] += prev[s - start];
            }
        }
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 20) return 0;              /* values below the offset are irrelevant */
    ensure_computed(n);
    return dp[10][(size_t)n];
}
