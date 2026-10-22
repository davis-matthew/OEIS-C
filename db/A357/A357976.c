/*
 * A357976  Numbers with a divisor having the same sum of prime indices as
 *          their quotient.
 * https://oeis.org/A357976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 2744, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the nth positive integer that admits a divisor d such that the sum of the
   prime indices of d equals the sum of the prime indices of N/d.  */

/*  The code below implements the sequence via brute‑force search.
    Primes up to 1,000,000 are generated once, and each tested
    integer is factorised and the multiset of prime indices is
    partitioned by a subset–sum bitset.  The results are cached
    so repeated calls with arbitrary n are fast.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/*                         Prime generation                              */
/* --------------------------------------------------------------------- */
#define PRIME_LIMIT 1000000U   /* enough for the required range */

static uint32_t *primes     = NULL;
static size_t   primes_cnt  = 0;
static uint32_t *prime_index = NULL;   /* 0 for non‑prime, else 1‑based index */

static void init_primes(void)
{
    static bool initialized = false;
    if (initialized) return;

    /* Allocate prime_index table */
    prime_index = (uint32_t *)calloc((size_t)(PRIME_LIMIT + 1), sizeof(uint32_t));
    if (!prime_index) exit(EXIT_FAILURE);

    /* Basic sieve of Eratosthenes */
    unsigned char *is_prime = (unsigned char *)calloc((size_t)(PRIME_LIMIT + 1), sizeof(unsigned char));
    if (!is_prime) exit(EXIT_FAILURE);
    memset(is_prime, 1, (size_t)(PRIME_LIMIT + 1));
    is_prime[0] = is_prime[1] = 0;

    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;

    /* Build primes list and index map */
    size_t cap = 128;
    primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);
    primes_cnt = 0;

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) {
            if (primes_cnt == cap) {
                cap <<= 1;
                primes = (uint32_t *)realloc(primes, cap * sizeof(uint32_t));
                if (!primes) exit(EXIT_FAILURE);
            }
            primes[primes_cnt++] = i;
            prime_index[i] = (uint32_t)(primes_cnt);   /* 1‑based */
        }

    free(is_prime);
    initialized = true;
}

/* --------------------------------------------------------------------- */
/* Bitset shift/or for subset-sum DP                                 */
/* --------------------------------------------------------------------- */
static void shift_or(uint64_t *dp, uint64_t *tmp, size_t words, uint32_t shift)
{
    size_t word_shift = shift >> 6;
    uint32_t bit_shift = shift & 63U;
    memset(tmp, 0, words * sizeof(uint64_t));

    for (size_t i = 0; i < words; ++i) {
        uint64_t w = dp[i];
        if (!w) continue;
        size_t j = i + word_shift;
        if (j < words) {
            if (bit_shift == 0) {
                tmp[j] |= w;
            } else {
                tmp[j] |= w << bit_shift;
                if (j + 1 < words)
                    tmp[j + 1] |= w >> (64 - bit_shift);
            }
        }
    }

    for (size_t i = 0; i < words; ++i)
        dp[i] |= tmp[i];
}

/* --------------------------------------------------------------------- */
/* Check whether a number satisfies the property                       */
/* --------------------------------------------------------------------- */
static bool is_valid(idx_t val)
{
    if (val == 1ULL) return true;   /* 1 always qualifies */

    init_primes();   /* ensure primes ready */

    /* Collect all prime indices (with multiplicity) */
    size_t idx_cap = 32;
    uint32_t *idx_arr = (uint32_t *)malloc(idx_cap * sizeof(uint32_t));
    if (!idx_arr) exit(EXIT_FAILURE);
    size_t idx_cnt = 0;
    idx_t tmp = val;
    idx_t sum = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p * (idx_t)p > tmp) break;
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            uint32_t idx = prime_index[p];
            for (idx_t k = 0; k < e; ++k) {
                if (idx_cnt == idx_cap) {
                    idx_cap <<= 1;
                    idx_arr = (uint32_t *)realloc(idx_arr, idx_cap * sizeof(uint32_t));
                    if (!idx_arr) exit(EXIT_FAILURE);
                }
                idx_arr[idx_cnt++] = idx;
                sum += idx;
            }
        }
    }

    if (tmp > 1) {   /* remaining prime factor > sqrt(val) */
        uint32_t idx = prime_index[(uint32_t)tmp];
        for (idx_t k = 0; k < 1; ++k) {
            if (idx_cnt == idx_cap) {
                idx_cap <<= 1;
                idx_arr = (uint32_t *)realloc(idx_arr, idx_cap * sizeof(uint32_t));
                if (!idx_arr) exit(EXIT_FAILURE);
            }
            idx_arr[idx_cnt++] = idx;
            sum += idx;
        }
    }

    if (sum & 1U) { free(idx_arr); return false; }  /* odd sum cannot split */

    idx_t target = sum >> 1;
    size_t words = (size_t)(target >> 6) + 1;

    uint64_t *dp  = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *tmpb = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!dp || !tmpb) { free(dp); free(tmpb); free(idx_arr); exit(EXIT_FAILURE); }

    dp[0] = 1ULL;   /* sum 0 reachable */

    for (size_t i = 0; i < idx_cnt; ++i)
        shift_or(dp, tmpb, words, idx_arr[i]);

    bool ok = ((dp[(size_t)(target >> 6)] >> (target & 63U)) & 1ULL) != 0;

    free(dp);
    free(tmpb);
    free(idx_arr);

    return ok;
}

/* --------------------------------------------------------------------- */
/* Sequence function                                                   */
/* --------------------------------------------------------------------- */
static idx_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 0;

seq_t a(idx_t n)
{
    init_primes();    /* ensure primes ready */

    while (cache_len < (size_t)n) {
        ++last_checked;
        if (is_valid(last_checked)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                seq_cache = (idx_t *)realloc(seq_cache, cache_cap * sizeof(idx_t));
                if (!seq_cache) exit(EXIT_FAILURE);
            }
            seq_cache[cache_len++] = last_checked;
        }
    }
    return (seq_t)seq_cache[(size_t)n - 1];
}
