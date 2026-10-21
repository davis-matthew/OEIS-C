/*
 * A291109  Numbers that are not the sum of the squarefree divisors of some
 *          natural number.
 * https://oeis.org/A291109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 185, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291109 – numbers that are not representable as a product
   of distinct (p+1), p prime.  The implementation builds a boolean
   array of representable numbers up to a fixed limit using a 0‑1 subset
   product DP, then returns the n‑th non‑representable value. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/build_primes_upto.h"

static bool *is_rep      = NULL;   /* is_rep[i] == true  ⇔  i is representable */
static size_t rep_limit  = 0;       /* maximum index of is_rep */
static uint64_t *primes  = NULL;   /* list of primes up to rep_limit */
static size_t primes_cnt = 0;

static seq_t *cache      = NULL;   /* cached non‑representables */
static size_t cache_cap  = 0;
static idx_t  cache_len  = 0;

static idx_t search_val        = 2; /* next integer to test */
static bool   representation_ready = false;

static void init_representation(idx_t limit) {
    rep_limit = limit;
    size_t size = (size_t)limit + 1;
    is_rep = (bool *)malloc(size * sizeof(bool));
    memset(is_rep, 0, size);
    is_rep[1] = true;

    build_primes_upto(limit, &primes, &primes_cnt);

    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        idx_t q = (idx_t)primes[pi] + 1;
        if (q > limit) break;
        idx_t max_mul = limit / q;
        for (idx_t val = max_mul; val >= 1; --val) {
            if (is_rep[(size_t)val]) {
                idx_t prod = val * q;
                is_rep[(size_t)prod] = true;
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (!representation_ready) {
        init_representation(200000);  /* covers n up to ~100 000 */
        representation_ready = true;
    }
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    while (cache_len < n) {
        while (search_val <= rep_limit && is_rep[(size_t)search_val]) {
            ++search_val;
        }
        /* search_val is now non‑representable */
        cache[cache_len++] = (seq_t)search_val;
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        ++search_val;
    }
    return cache[(size_t)(n - 1)];
}
