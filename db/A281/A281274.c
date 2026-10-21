/*
 * A281274  Expansion of Product_{j>=1} (1 + x^(Sum_{i=1..j} prime(i))).
 * https://oeis.org/A281274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{j≥1} (1 + x^(p1+…+pj)) where p1,
 *            p2, … are the prime numbers. Equivalently, a(n) is the number of
 *            partitions of n into distinct partial sums of primes
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

/* static cache of partial sums of primes (stored as idx_t to avoid overflow) */
static idx_t  *sums = NULL;        /* array of cumulative prime sums */
static size_t sums_cnt = 0;        /* number of elements stored */
static size_t sums_cap = 0;        /* allocated capacity of the array   */
static idx_t  last_prime = 0;      /* last prime that was added */
static idx_t  cum_sum   = 0;       /* current cumulative sum of primes */

/* Ensure that all partial sums <= n are present in the cache.  */
static void ensure_sums_up_to(idx_t n)
{
    while (true)
    {
        /* obtain next prime */
        idx_t p = (last_prime == 0) ? 2 : next_prime(last_prime);
        idx_t next_cum = cum_sum + p;
        if (next_cum > n) break;          /* all needed sums have been cached */

        /* grow the array if necessary */
        if (sums_cnt == sums_cap)
        {
            size_t new_cap = (sums_cap == 0) ? 4 : sums_cap * 2;
            idx_t *tmp = (idx_t *)realloc(sums, new_cap * sizeof(idx_t));
            if (tmp == NULL) abort();     /* out of memory */
            sums = tmp;
            sums_cap = new_cap;
        }

        sums[sums_cnt++] = next_cum;
        cum_sum = next_cum;
        last_prime = p;
    }
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Build the partial‑sum list up to n.  */
    ensure_sums_up_to(n);

    /* Allocate DP array of size n+1, zero‑initialised.  */
    size_t dp_size = (size_t)(n + 1);
    seq_t *dp = (seq_t *)calloc(dp_size, sizeof(seq_t));
    if (dp == NULL) abort();

    dp[0] = 1;   /* empty subset */

    /* For each partial sum, perform the standard subset‑sum DP. */
    for (size_t i = 0; i < sums_cnt; ++i)
    {
        idx_t s = sums[i];
        if (s > n) break; /* safety, though should not happen */
        for (idx_t sum = n; sum >= s; --sum)
        {
            dp[sum] += dp[sum - s];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
