/*
 * A368100  Numbers of which it is possible to choose a different prime factor
 *          of each prime index.
 * https://oeis.org/A368100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368100: Numbers n for which each prime index appearing in the
 * factorisation of n can be assigned a distinct prime factor of that index. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"

#define MAX_PRIME_LIMIT (2000000)
#define PRIMES_ARRAY_SIZE 200000     /* safely > π(2 000 000) */

static uint32_t primes[PRIMES_ARRAY_SIZE];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt != 0) return;
    uint8_t *sieve = (uint8_t*)calloc(MAX_PRIME_LIMIT + 1, 1);
    if (!sieve) abort();
    for (uint32_t i = 2; i <= MAX_PRIME_LIMIT; ++i) sieve[i] = 1;
    for (uint32_t i = 2; (uint64_t)i * i <= MAX_PRIME_LIMIT; ++i)
        if (sieve[i])
            for (uint64_t j = (uint64_t)i * i; j <= MAX_PRIME_LIMIT; j += i)
                sieve[(size_t)j] = 0;
    for (uint32_t i = 2; i <= MAX_PRIME_LIMIT && prime_cnt < PRIMES_ARRAY_SIZE; ++i)
        if (sieve[i]) primes[prime_cnt++] = i;
    free(sieve);
}

static size_t prime_index(uint32_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p) lo = mid + 1;
        else                 hi = mid;
    }
    return lo;                    /* 0‑based index of prime p */
}

#define MAX_INST 512
#define MAX_PF   12

static uint32_t instance_idx[MAX_INST];
static uint32_t idx_pf[MAX_INST][MAX_PF];
static size_t     idx_pf_cnt[MAX_INST];

static void factor_distinct(uint32_t n, uint32_t *pf, size_t *cnt)
{
    *cnt = 0;
    uint32_t tmp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            pf[(*cnt)++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) pf[(*cnt)++] = tmp;
}

#define USED_PRIME_MAX (MAX_PRIME_LIMIT)

static bool used_prime[USED_PRIME_MAX + 1];

static int match_k;

static bool dfs_match(int pos)
{
    if (pos == match_k) return true;
    for (size_t j = 0; j < idx_pf_cnt[pos]; ++j) {
        uint32_t p = idx_pf[pos][j];
        if (!used_prime[p]) {
            used_prime[p] = true;
            if (dfs_match(pos + 1)) return true;
            used_prime[p] = false;
        }
    }
    return false;
}

static bool candidate_ok(idx_t cand)
{
    if (cand == 1) return true;

    uint64_t pf[64], ex[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)cand, pf, ex, &pf_cnt);

    size_t inst = 0;
    for (size_t i = 0; i < pf_cnt && inst < MAX_INST; ++i) {
        uint32_t p = (uint32_t)pf[i];
        uint32_t idx = (uint32_t)(prime_index(p) + 1);   /* 1‑based prime index */
        for (uint64_t t = 0; t < ex[i] && inst < MAX_INST; ++t)
            instance_idx[inst++] = idx;
    }
    if (inst == 0) return true;

    for (size_t i = 0; i < inst; ++i) {
        uint32_t idx_val = instance_idx[i];
        factor_distinct(idx_val, idx_pf[i], &idx_pf_cnt[i]);
        if (idx_pf_cnt[i] == 0) return false;
    }

    memset(used_prime, 0, sizeof(used_prime));
    match_k = (int)inst;
    return dfs_match(0);
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_candidate = 1;

static void ensure_cache(idx_t n)
{
    init_primes();
    while (cache_len < n) {
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();
            cache = new_cache;
            cache_cap = new_cap;
        }
        if (candidate_ok(next_candidate))
            cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n - 1];
}
