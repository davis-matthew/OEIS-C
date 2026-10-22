/*
 * A325370  Numbers whose prime signature has multiplicities covering an
 *          initial interval of positive integers.
 * https://oeis.org/A325370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer whose prime signature has multiplicities
 *            covering an initial interval of positive integers
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   array_append
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/array_append.h"

/* The types seq_t and idx_t are provided by the build environment. */

/* Factor a number into distinct primes and store the exponents in
   `exps`.  The array may hold up to 64 exponents.  `cnt` receives the
   number of distinct prime factors. */
static void factor_exponents(uint64_t n, uint64_t exps[64], size_t *cnt)
{
    size_t c = 0;
    uint64_t rem = n;

    /* Handle factor 2 separately. */
    if ((rem & 1ULL) == 0) {
        uint64_t e = 0;
        while ((rem & 1ULL) == 0) {
            rem >>= 1;
            ++e;
        }
        exps[c++] = e;
    }

    /* Now test odd factors. */
    for (uint64_t p = 3; p <= rem / p; p += 2) {
        if (rem % p == 0) {
            uint64_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            exps[c++] = e;
        }
    }

    /* If anything remains, it is a prime factor with exponent 1. */
    if (rem > 1)
        exps[c++] = 1;

    *cnt = c;
}

/* Decide whether an integer meets the sequence condition. */
static bool qualifies(uint64_t x)
{
    if (x == 1)
        return true;   /* 1 is always accepted. */

    uint64_t exps[64];
    size_t cnt;
    factor_exponents(x, exps, &cnt);

    /* Collect counts of each distinct exponent. */
    size_t count_vals[64];
    size_t count_vals_cnt = 0;
    unsigned int max_count = 0;

    for (size_t i = 0; i < cnt; ++i) {
        /* Skip if this exponent was processed already. */
        bool processed = false;
        for (size_t j = 0; j < i; ++j)
            if (exps[j] == exps[i]) {
                processed = true;
                break;
            }
        if (processed)
            continue;

        /* Count occurrences of this exponent. */
        unsigned int oc = 0;
        for (size_t j = 0; j < cnt; ++j)
            if (exps[j] == exps[i])
                ++oc;

        count_vals[count_vals_cnt++] = oc;
        if (oc > max_count)
            max_count = oc;
    }

    /* Check that the set of multiplicity counts is exactly {1,2,…,max_count}. */
    bool seen[65] = { false };   /* max_count is at most 64 for 64‑bit inputs. */
    for (size_t i = 0; i < count_vals_cnt; ++i) {
        unsigned int c = (unsigned int)count_vals[i];
        if (c == 0 || c > max_count)
            return false;
        seen[c] = true;
    }
    for (unsigned int i = 1; i <= max_count; ++i)
        if (!seen[i])
            return false;
    return true;
}

/* The cached sequence.  Index 0 corresponds to a(1). */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 2;

/* Extend the cache until it contains at least `n` elements. */
static void cache_extend(idx_t n)
{
    if (cache_size == 0) {          /* First use: store the term for a(1)=1 */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = (seq_t)1;
        cache_size = 1;
        cache_cap = 1;
        next_candidate = 2;
    }

    while (cache_size < n) {
        uint64_t cand = next_candidate++;
        if (qualifies(cand))
            array_append(&cache, &cache_size, &cache_cap, (seq_t)cand);
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;             /* undefined for n=0, but safe return */
    cache_extend(n);
    return cache[n - 1];
}
