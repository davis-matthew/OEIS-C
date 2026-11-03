/*
 * A332282  Numbers whose unsorted prime signature is not unimodal.
 * https://oeis.org/A332282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer whose unsorted prime exponents
 *            sequence (prime signature) is NOT unimodal. The function scans
 *            the integers sequentially and caches the values, so calls may
 *            come in any order
 * Range:     u8 none, u16 n <= 565, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9187 (2026-11-03)
 * Helpers:   build_primes_upto, factor_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper headers provided by the build system. */
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/isqrt.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Return true iff the sequence of exponents (unsorted prime signature)
   is unimodal (weakly increasing then weakly decreasing). */
static bool is_unimodal(const uint64_t *exps, size_t sz)
{
    if (sz <= 2)
        return true;
    size_t i = 0;
    while (i + 1 < sz && exps[i] <= exps[i + 1])
        ++i;                /* weakly increasing part */
    while (i + 1 < sz && exps[i] >= exps[i + 1])
        ++i;                /* weakly decreasing part */
    return i == sz - 1;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    /* Static state for memoisation and prime handling. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static uint64_t *primes      = NULL;
    static size_t primes_cnt = 0;
    static uint64_t prime_limit = 0;
    static uint64_t next_candidate = 1;   /* start scanning from 1 */

    /* Generate terms until we have at least 'n' cached. */
    while (cache_len < (size_t)n)
    {
        /* Ensure prime list covers sqrt(next_candidate). */
        uint64_t limit = isqrt((idx_t)next_candidate) + 1;
        if (limit > prime_limit)
        {
            free(primes);                     /* discard old list */
            primes = NULL;
            build_primes_upto(limit, &primes, &primes_cnt);
            prime_limit = limit;
        }

        /* Factor the candidate and collect exponents. */
        uint64_t exps[64];
        size_t distinct = 0;
        factor_u64(next_candidate, primes, exps, &distinct);

        /* If the exponent list is NOT unimodal, cache the candidate. */
        if (!is_unimodal(exps, distinct))
        {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
