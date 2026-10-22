/*
 * A317590  Heinz numbers of integer partitions that are not uniformly normal.
 * https://oeis.org/A317590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computes OEIS A317590 – Heinz numbers of non‑uniformly normal
 *            integer partitions
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_uniform_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_uniform_rec.h"

/* Constants for the sieve and storage limits */
#define LIMIT      2000000      /* Sieve limit – enough for the required n */
#define MAX_PRIMES 160000       /* Number of primes up to LIMIT (safety margin) */
#define MAX_FACTORS 64          /* Max distinct prime factors expected for tested numbers */

/* Tables for the sieve */
static uint32_t spf[LIMIT + 1];            /* Smallest prime factor for each number */
static uint32_t prime_index[LIMIT + 1];     /* 1‑based index of prime value */
static uint32_t primes[MAX_PRIMES];
static size_t prime_count = 0;

/* Sequence storage (capacity chosen to cover at least n = 100 000) */
#define SEQ_CAP 100000
static seq_t seq[SEQ_CAP];
static size_t seq_len = 0;

/* Global state for incremental generation */
static uint64_t next_candidate = 2;
static bool initialized = false;

/* Forward declarations */
static void init_sieve(void);
static void factor_number(uint64_t n, uint32_t idx_arr[MAX_FACTORS],
                         uint32_t exp_arr[MAX_FACTORS], unsigned int *cnt);
static int is_uniform_partition(const uint32_t idx_arr[MAX_FACTORS],
                                const uint32_t exp_arr[MAX_FACTORS],
                                unsigned int cnt);
static int is_uniform_rec(const int *arr, int len);

/* Build the sieve of Eratosthenes, spf table, and prime index mapping */
static void init_sieve(void)
{
    if (initialized) return;
    initialized = true;

    /* Initialize spf: each number’s smallest prime factor is itself */
    for (uint32_t i = 2; i <= LIMIT; ++i) spf[i] = i;
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (spf[i] == i) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    /* Extract primes and build the 1‑based prime index table */
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == i) {
            primes[prime_count] = i;
            prime_index[i] = (uint32_t)(prime_count + 1);
            ++prime_count;
        }
    }
}

/* Factorize n using the spf table; store prime indices and exponents */
static void factor_number(uint64_t n, uint32_t idx_arr[MAX_FACTORS],
                          uint32_t exp_arr[MAX_FACTORS], unsigned int *cnt)
{
    unsigned int k = 0;
    while (n > 1 && k < MAX_FACTORS) {
        uint32_t p;
        if (n <= LIMIT) {
            p = spf[(uint32_t)n];
        } else {
            /* Trial division using precomputed primes */
            p = 0;
            for (size_t i = 0; i < prime_count; ++i) {
                uint32_t prime = primes[i];
                if ((uint64_t)prime * prime > n) break;
                if (n % prime == 0) { p = prime; break; }
            }
            if (p == 0) p = (uint32_t)n;   /* n itself is prime */
        }
        uint32_t e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        idx_arr[k] = prime_index[p];
        exp_arr[k] = e;
        ++k;
    }
    *cnt = k;
}

/* Test whether the factorisation of a number corresponds to a uniformly normal partition */
static int is_uniform_partition(const uint32_t idx_arr[MAX_FACTORS],
                                const uint32_t exp_arr[MAX_FACTORS],
                                unsigned int cnt)
{
    /* Determine maximum part value (prime index) */
    uint32_t maxIdx = 0;
    for (unsigned int i = 0; i < cnt; ++i)
        if (idx_arr[i] > maxIdx) maxIdx = idx_arr[i];

    /* Build exponent array per part value (index 1‑based → array 0‑based) */
    int exps[MAX_FACTORS] = {0};
    for (unsigned int i = 0; i < cnt; ++i)
        exps[idx_arr[i] - 1] = (int)exp_arr[i];

    return is_uniform_rec(exps, (int)maxIdx);
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (!initialized) init_sieve();

    while (seq_len < n) {
        uint32_t idx_arr[MAX_FACTORS];
        uint32_t exp_arr[MAX_FACTORS];
        unsigned int cnt = 0;

        factor_number(next_candidate, idx_arr, exp_arr, &cnt);
        if (!is_uniform_partition(idx_arr, exp_arr, cnt))
            seq[seq_len++] = (seq_t)next_candidate;

        ++next_candidate;
    }
    return seq[n - 1];
}
