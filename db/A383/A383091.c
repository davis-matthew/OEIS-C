/*
 * A383091  Numbers whose prime indices have at most one permutation with all
 *          equal run-lengths.
 * https://oeis.org/A383091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers whose prime indices have at most one permutation with
 *            all equal run-lengths. The function a(idx_t n) returns the nth
 *            term (offset 1)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   backtrack_counts, gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/backtrack_counts.h"
#include "../utils/gcd_int.h"

/* Prime generation (simple sieve) */
#define PRIME_LIMIT 2000000U   /* enough for the first ~10000 terms */

static uint32_t *prime_list = NULL;
static size_t    prime_cnt  = 0;

static void init_primes(void)
{
    if (prime_list != NULL)
        return;   /* already initialised */

    bool *is_composite = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_composite)
        exit(EXIT_FAILURE);

    size_t est = PRIME_LIMIT / 10;      /* rough over‑estimate */
    prime_list = (uint32_t *)malloc(est * sizeof(uint32_t));
    if (!prime_list)
        exit(EXIT_FAILURE);

    for (uint32_t p = 2; p <= PRIME_LIMIT; ++p)
    {
        if (!is_composite[p])
        {
            prime_list[prime_cnt++] = p;
            if ((uint64_t)p * p <= PRIME_LIMIT)
            {
                for (uint32_t q = p * p; q <= PRIME_LIMIT; q += p)
                    is_composite[q] = true;
            }
        }
    }
    free(is_composite);
}

/* Factorisation: fill exponents array, return count */
static size_t factor_uint64(uint64_t n, uint8_t *exponents)
{
    size_t cnt = 0;
    if (n <= 1)
        return 0;

    init_primes();

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= n; ++i)
    {
        uint32_t p = prime_list[i];
        if (n % p == 0)
        {
            uint8_t e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            exponents[cnt++] = e;
        }
    }
    if (n > 1)      /* remaining prime factor */
    {
        exponents[cnt++] = 1;
    }
    return cnt;
}

/* Euclidean gcd for int */

/* Test whether n satisfies the definition for sequence A383091. */
static bool valid_term(uint64_t n)
{
    if (n == 1)
        return true;

    uint8_t exponents[64];
    size_t exp_cnt = factor_uint64(n, exponents);
    if (exp_cnt == 0)
        return true;   /* prime or 1 */

    /* Compute gcd of exponents */
    int g = exponents[0];
    for (size_t i = 1; i < exp_cnt; ++i)
        g = gcd_int(g, exponents[i]);

    /* Enumerate all divisors of g */
    int divisors[64];
    size_t dcnt = 0;
    for (int i = 1; i * i <= g; ++i)
    {
        if (g % i == 0)
        {
            divisors[dcnt++] = i;
            if (i != g / i)
                divisors[dcnt++] = g / i;
        }
    }

    int total_found = 0;
    int runs[64];

    for (size_t di = 0; di < dcnt; ++di)
    {
        int k = divisors[di];
        int total_runs = 0;
        for (size_t i = 0; i < exp_cnt; ++i)
        {
            runs[i] = exponents[i] / k;
            total_runs += runs[i];
        }
        if (total_runs == 0)
            continue;

        int found = 0;
        backtrack_counts(runs, (int)exp_cnt, -1, 0, total_runs, &found);
        if (found >= 2)
            return false;
        total_found += found;
        if (total_found > 1)
            return false;
    }
    return true;
}

/* -------------------------------------------------------------- */
/* Sequence cache and main function                              */
/* -------------------------------------------------------------- */

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap  = 0;
static uint64_t next_candidate = 1;

static void ensure_cache_capacity(idx_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < min_cap)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* safety */

    /* Expand sequence until we have at least n terms */
    while (cache_size < n)
    {
        if (valid_term(next_candidate))
        {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
