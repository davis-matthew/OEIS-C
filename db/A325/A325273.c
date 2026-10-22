/*
 * A325273  Prime omicron of n!.
 * https://oeis.org/A325273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime omicron of n! – the second‑to‑last term of the
 *            ω‑sequence of n! (see OEIS A325273). Returns 0 for n≤1, 1 if n!
 *            is prime, otherwise the previous Ω value obtained by iterating
 *            the red function until a prime is reached
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

#define PRIME_LIMIT 200000  /* > prime(10000) ≈ 104729 */

/* Global state for primes and prime indices */
static idx_t *primes = NULL;        /* array of primes up to PRIME_LIMIT */
static size_t primes_cnt = 0;       /* number of primes */
static int *prime_index = NULL;     /* prime_index[prime] = index in primes[] */
static bool primes_inited = false;

/* One‑time initialization of primes and index table */
static void init_primes(void)
{
    if (primes_inited) return;

    /* Build the list of primes <= PRIME_LIMIT */
    build_primes(PRIME_LIMIT, (idx_t **)&primes, &primes_cnt);

    /* Allocate mapping from prime value to its index */
    prime_index = (int *)calloc(PRIME_LIMIT + 1, sizeof(int));
    if (!prime_index) exit(EXIT_FAILURE);
    for (int i = 0; i <= (int)PRIME_LIMIT; ++i) prime_index[i] = -1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        prime_index[primes[i]] = (int)i;
    }

    primes_inited = true;
}

/* Compute prime omicron of n! */
static seq_t prime_omicron_factorial(idx_t n)
{
    if (n <= 1)  /* 0! = 1! = 1 → omicron 0 */
        return (seq_t)0;

    init_primes();

    /* Allocate exponent arrays (zero‑initialized) */
    uint32_t *curr_exp = (uint32_t *)calloc(primes_cnt, sizeof(uint32_t));
    uint32_t *next_exp = (uint32_t *)calloc(primes_cnt, sizeof(uint32_t));
    if (!curr_exp || !next_exp) exit(EXIT_FAILURE);

    /* Compute exponents of primes in n! (Legendre's formula) */
    uint32_t sum_exp = 0;            /* Ω(n!) */
    for (size_t i = 0; i < primes_cnt && primes[i] <= n; ++i) {
        uint32_t p = (uint32_t)primes[i];
        uint32_t e = 0;
        uint64_t q = n / p;
        while (q) { e += (uint32_t)q; q /= p; }
        curr_exp[i] = e;
        sum_exp += e;
    }

    /* Fast‑check: n! is prime only for 2! = 2 */
    size_t nz = 0;
    for (size_t i = 0; i < primes_cnt; ++i)
        if (curr_exp[i]) ++nz;

    if (nz == 1 && curr_exp[0] == 1) {  /* only 2¹ */
        free(curr_exp); free(next_exp);
        return (seq_t)1;
    }

    uint32_t prev_omega = sum_exp;

    /* Iterate red until a prime is reached */
    while (1) {
        /* Find maximum exponent in current factorisation */
        uint32_t max_exp = 0;
        for (size_t i = 0; i < primes_cnt; ++i)
            if (curr_exp[i] > max_exp) max_exp = curr_exp[i];

        /* Frequency of each exponent */
        uint32_t *freq = (uint32_t *)calloc(max_exp + 1, sizeof(uint32_t));
        if (!freq) exit(EXIT_FAILURE);
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t e = curr_exp[i];
            if (e) ++freq[e];
        }

        /* Build next exponent vector and compute new Ω */
        uint32_t new_omega = 0;
        memset(next_exp, 0, primes_cnt * sizeof(uint32_t));

        for (uint32_t e = 1; e <= max_exp; ++e) {
            uint32_t f = freq[e];
            if (!f) continue;
            /* prime(e) = primes[e-1] (1‑based). */
            uint32_t p_e = primes[e - 1];
            int idx = prime_index[p_e];
            if (idx < 0) {
                free(freq);
                free(curr_exp);
                free(next_exp);
                exit(EXIT_FAILURE);
            }
            next_exp[idx] = f;
            new_omega += f;
        }
        free(freq);

        /* Check if the next state would be a prime (single 1) */
        uint32_t nz_next = 0, exp_next = 0;
        for (size_t i = 0; i < primes_cnt; ++i)
            if (next_exp[i]) { ++nz_next; exp_next = next_exp[i]; }

        if (nz_next == 1 && exp_next == 1) {
            free(curr_exp);
            free(next_exp);
            return (seq_t)prev_omega;
        }

        /* Prepare for next iteration */
        memcpy(curr_exp, next_exp, primes_cnt * sizeof(uint32_t));
        prev_omega = new_omega;
    }
}

/* Sequence function required by the problem statement */
seq_t a(idx_t n)
{
    return prime_omicron_factorial(n);
}
