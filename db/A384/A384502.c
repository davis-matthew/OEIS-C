/*
 * A384502  Maximum number of distinct prime factors in an n-digit number, n >
 *          3, where its set of distinct prime factors can be partitioned into
 *          two equal-sum subsets, each containing at least two elements.
 * https://oeis.org/A384502
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 692, u16 n <= 704, u32 n <= 704, u64 n <= 704,
 *            u128 n <= 704, big any n
 * Helpers:   max_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = maximum k such that the product of a partitionable set of
 * k distinct primes is less than 10^n.  For odd k the minimal such product
 * is the primorial P_k = p_1 · p_2 · … · p_k.  
 * For even k the minimal such product uses only odd primes:
 * P_k^(even) = q_1 · q_2 · … · q_k where q_i is the i‑th odd prime.
 * a(n) is the larger of the maximal odd k and maximal even k constrained
 * by these products.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/max_index.h"

static uint32_t *g_primes_all = NULL;    /* primes in ascending order */
static size_t    g_prime_cnt   = 0;       /* total number of primes */

/* log10 of primorial of all primes up to i (1‑based) */
static double *g_log_all  = NULL;
static size_t  g_log_all_cnt  = 0;

/* log10 of primorial of first i odd primes (1‑based) */
static double *g_log_odd  = NULL;
static size_t  g_log_odd_cnt  = 0;

/* simple sieve of Eratosthenes up to LIMIT */
static void sieve_primes(uint32_t LIMIT)
{
    uint8_t *is_comp = (uint8_t *)calloc(LIMIT + 1, 1);
    if (!is_comp) abort();

    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = 1;

    /* count primes */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++cnt;

    g_primes_all = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!g_primes_all) abort();

    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) g_primes_all[idx++] = i;

    g_prime_cnt = cnt;

    free(is_comp);
}

static void init_tables(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Sieve up to a convenient limit – enough primes for n up to ~10000. */
    const uint32_t SIEVE_LIMIT = 200000;      /* contains > 18000 primes   */
    sieve_primes(SIEVE_LIMIT);

    /* Build prefix sums of log10 for all primes. */
    g_log_all_cnt = g_prime_cnt;
    g_log_all = (double *)malloc((g_log_all_cnt + 1) * sizeof(double));
    if (!g_log_all) abort();
    g_log_all[0] = 0.0;
    for (size_t i = 1; i <= g_log_all_cnt; ++i)
        g_log_all[i] = g_log_all[i - 1] + log10((double)g_primes_all[i - 1]);

    /* Build prefix sums for odd primes (exclude 2). */
    /* Count odd primes */
    g_log_odd_cnt = 0;
    for (size_t i = 1; i < g_prime_cnt; ++i)  /* skip index 0 (prime 2) */
        if (g_primes_all[i] % 2 == 1) ++g_log_odd_cnt;

    g_log_odd = (double *)malloc((g_log_odd_cnt + 1) * sizeof(double));
    if (!g_log_odd) abort();
    g_log_odd[0] = 0.0;
    size_t odd_idx = 0;
    for (size_t i = 1; i < g_prime_cnt; ++i) {
        if (g_primes_all[i] % 2 == 1) {
            ++odd_idx;
            g_log_odd[odd_idx] = g_log_odd[odd_idx - 1] + log10((double)g_primes_all[i]);
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)5;   /* behaviour below offset irrelevant */

    init_tables();

    double nd = (double)n;

    /* largest odd k (include prime 2). */
    size_t max_k_all = max_index(g_log_all, g_log_all_cnt, nd);
    size_t odd_k = (max_k_all % 2 == 1) ? max_k_all : ((max_k_all >= 1) ? max_k_all - 1 : 0);

    /* largest even k (only odd primes). */
    size_t max_k_odd = max_index(g_log_odd, g_log_odd_cnt, nd);
    size_t even_k = 0;
    if (max_k_odd >= 2) {
        even_k = (max_k_odd % 2 == 0) ? max_k_odd : max_k_odd - 1;
    }

    size_t result = odd_k > even_k ? odd_k : even_k;
    if (result < 5) result = 5;
    return (seq_t)result;
}
