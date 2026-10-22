/*
 * A322847  Numbers whose prime indices have no equivalent primes.
 * https://oeis.org/A322847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This sequence has no simple closed form. It enumerates the
 *            positive integers whose multiset of prime indices contains no
 *            pair of equivalent primes. The test for a number n is performed
 *            by factorising n, collecting the indices of its prime factors
 *            (with multiplicity), and checking whether any two...
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   factor_u64, free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

#define SIEVE_LIMIT     200000
#define MAX_IDX_CNT      128
#define MAX_R_PRIMES      64

/* ----- Prime index table ----- */
static uint32_t prime_index_map[SIEVE_LIMIT + 1];
static bool  primes_built = false;

static void init_prime_index_map(void)
{
    memset(prime_index_map, 0, sizeof(prime_index_map));
    static uint8_t sieve[SIEVE_LIMIT + 1];
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) sieve[i] = 0;
    for (uint32_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i])
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = 1;
    }
    uint32_t idx = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!sieve[i])
            prime_index_map[i] = ++idx;   /* 1‑based indices */
    primes_built = true;
}

static inline uint32_t prime_index(uint32_t p)
{
    return (p <= SIEVE_LIMIT) ? prime_index_map[p] : 0;
}

/* ----- Test whether a number n is in the sequence ----- */
static bool has_equivalent_primes(uint64_t n)
{
    if (n == 1) return true;           /* 1 always qualifies */

    /* Factor n */
    uint64_t pn[32], en[32];
    size_t pcnt;
    factor_u64(n, pn, en, &pcnt);
    if (pcnt == 0) return true;

    /* Build list of prime indices (with multiplicity) */
    uint32_t idx_list[MAX_IDX_CNT];
    size_t idx_cnt = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint32_t idx = prime_index((uint32_t)pn[i]);
        for (uint32_t k = 0; k < en[i]; ++k)
            if (idx_cnt < MAX_IDX_CNT)
                idx_list[idx_cnt++] = idx;   /* idx may be 1 … SIEVE_LIMIT */
    }
    if (idx_cnt == 0) return true;

    /* Gather all distinct primes that appear in factorisations of indices */
    uint32_t r_primes[MAX_R_PRIMES];
    uint8_t   r_exps [MAX_R_PRIMES][MAX_IDX_CNT];
    size_t    r_cnt = 0;

    /* initialise exponent tables to zero */
    for (size_t i = 0; i < MAX_R_PRIMES; ++i)
        for (size_t j = 0; j < idx_cnt; ++j)
            r_exps[i][j] = 0;

    for (size_t pos = 0; pos < idx_cnt; ++pos) {
        uint32_t idx_val = idx_list[pos];
        if (idx_val <= 1) continue;          /* 1 has no prime factors */

        uint64_t rpn[32], ren[32];
        size_t   rcnt;
        factor_u64(idx_val, rpn, ren, &rcnt);

        for (size_t t = 0; t < rcnt; ++t) {
            uint32_t r  = (uint32_t)rpn[t];
            uint8_t  exp = (uint8_t)ren[t];

            /* find or create entry for prime r */
            size_t rpos = 0;
            while (rpos < r_cnt && r_primes[rpos] != r) ++rpos;
            if (rpos == r_cnt) {                /* new prime */
                if (r_cnt < MAX_R_PRIMES) {
                    r_primes[r_cnt] = r;
                    /* exponents already zero-initialised */
                    rpos = r_cnt++;
                }
            }
            r_exps[rpos][pos] = exp;
        }
    }

    /* Compare exponent patterns of distinct primes */
    for (size_t i = 0; i < r_cnt; ++i) {
        for (size_t j = i + 1; j < r_cnt; ++j) {
            bool equal = true;
            for (size_t k = 0; k < idx_cnt; ++k) {
                if (r_exps[i][k] != r_exps[j][k]) {
                    equal = false;
                    break;
                }
            }
            if (equal) return false;   /* equivalent primes detected */
        }
    }
    return true;
}

/* ----- Public sequence function ----- */
seq_t a(idx_t n)
{
    if (!primes_built) init_prime_index_map();

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 1;

    if (!cache) {                /* initial allocation */
        cache_cap = 32;
        cache = seq_alloc_new_array(cache_cap);
    }

    while (cache_len < (size_t)n) {
        while (!has_equivalent_primes(next_candidate))
            ++next_candidate;

        seq_t val = (seq_t)next_candidate;

        if (cache_len >= cache_cap) {          /* grow cache */
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = seq_alloc_new_array(new_cap);
            for (size_t i = 0; i < cache_len; ++i)
                tmp[i] = cache[i];
            free_seq_array(cache);
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = val;
        ++next_candidate;
    }

    return cache[n - 1];
}
