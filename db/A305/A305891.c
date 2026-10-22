/*
 * A305891  Filter sequence combining 2-adic valuation (A007814) and the prime
 *          signature (A046523) of n.
 * https://oeis.org/A305891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of the pair [v2(n), prime_signature(n)] as it first
 *            appears, assigning successive integers starting at 1
 * Range:     u8 n <= 9798, u16 n <= 9798, u32 n <= 9798, u64 n <= 9798,
 *            u128 n <= 9798, big any n
 * Helpers:   signature_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/signature_of.h"

typedef struct {
    uint8_t v2;               /* 2‑adic valuation of n */
    uint8_t len;              /* number of exponents in the signature */
    uint8_t ex[10];           /* exponents sorted in descending order */
} pair_t;

/* Global list of distinct pairs and their assigned ranks */
static pair_t *pair_list = NULL;
static size_t  pair_cap  = 0;
static size_t  pair_cnt  = 0;

/* Return the rank of the pair (v2, ex[0..len-1]).
   New pairs obtain the next rank value. */
static seq_t rank_of_pair(uint8_t v2, const uint8_t *ex, size_t len) {
    for (size_t i = 0; i < pair_cnt; ++i) {
        if (pair_list[i].v2 == v2 &&
            pair_list[i].len == (uint8_t)len &&
            memcmp(pair_list[i].ex, ex, len) == 0)
            return (seq_t)(i + 1);
    }

    /* New pair: grow the list if necessary */
    if (pair_cnt == pair_cap) {
        size_t new_cap = (pair_cap == 0) ? 64 : pair_cap << 1;
        pair_list = (pair_t *)realloc(pair_list, new_cap * sizeof(pair_t));
        pair_cap = new_cap;
    }

    pair_list[pair_cnt].v2   = v2;
    pair_list[pair_cnt].len  = (uint8_t)len;
    memcpy(pair_list[pair_cnt].ex, ex, len);

    return (seq_t)(++pair_cnt);
}

/* Cache of already computed sequence values */
static seq_t *cache = NULL;
static idx_t  cache_max = 0;

/* Public sequence function.
   Computes a(n) for all n >= 1, memoizing results. */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* safety, although offset is 1 */

    if (n > cache_max) {
        /* Allocate space for the new cache entries */
        idx_t new_cap = n + 1;   /* indices 0..n (we ignore 0) */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

        /* Compute sequence values up to n */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            uint64_t val = (uint64_t)i;

            /* 2‑adic valuation */
            uint8_t v2 = 0;
            uint64_t tmp = val;
            while ((tmp & 1ULL) == 0) { ++v2; tmp >>= 1; }

            /* Prime signature of val */
            uint8_t ex[10];
            size_t len;
            signature_of(val, ex, &len);

            cache[i] = rank_of_pair(v2, ex, len);
        }
        cache_max = n;
    }

    return cache[n];
}
