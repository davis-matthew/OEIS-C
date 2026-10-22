/*
 * A326785  BII-numbers of uniform regular set-systems.
 * https://oeis.org/A326785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer n (n ≥ 1) whose set of binary indices of
 *            the binary indices of n forms a uniform regular set-system.
 *            Computation is done by brute‑force generation with caching
 * Range:     u8 n <= 21, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: popcount for 64‑bit values */
static inline unsigned int popcount_u64(uint64_t x) {
    return (unsigned int)__builtin_popcountll(x);
}

/* Helper: returns true if the candidate integer n (0 ≤ n < 2^64) yields a uniform
   regular set-system when we take the binary indices of each binary index of n. */
static bool is_bii_valid_uint64(uint64_t n) {
    if (n == 0) return true;                      /* 0 corresponds to the empty set‑system */

    /* Collect the indices of the set bits in n (positions start at 1). */
    uint8_t pos[64];
    int len = 0;
    uint64_t tmp = n;
    int bitpos = 1;
    while (tmp) {
        if (tmp & 1ULL) pos[len++] = (uint8_t)bitpos;
        tmp >>= 1;
        ++bitpos;
    }

    /* Arrays to hold per‑element counts and to check uniformity. */
    unsigned int counts[64] = {0};
    int maxpos = 0;
    int first_size = -1;

    for (int idx = 0; idx < len; ++idx) {
        uint64_t i = (uint64_t)pos[idx];      /* i is the index value itself */
        unsigned int sz = popcount_u64(i);
        if (first_size == -1) first_size = sz;
        else if (sz != first_size) return false;

        /* Increment counts for each set bit in i. */
        uint64_t t = i;
        while (t) {
            unsigned int b = t & -t;                  /* lowest set bit */
            int bpos = __builtin_ctzll(t) + 1;         /* 1‑based position */
            ++counts[bpos - 1];
            t &= t - 1;                               /* clear lowest set bit */
        }

        if (i > (uint64_t)maxpos) maxpos = (int)i;
    }

    /* Check that all appearing elements occur the same number of times. */
    int target = -1;
    for (int j = 0; j < maxpos; ++j) {
        if (counts[j]) {
            if (target == -1) target = counts[j];
            else if (counts[j] != target) return false;
        }
    }
    return true;
}

/* Cached array of computed a(n) values. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_scan = 0;

/* Main sequence function as required. */
seq_t a(idx_t n) {
    /* Ensure we have at least n elements in the cache. */
    while (cache_len < (size_t)n) {
        /* Grow the cache if needed. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) exit(EXIT_FAILURE);   /* allocation failure */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Scan next candidate integer. */
        if (is_bii_valid_uint64(next_scan)) {
            cache[cache_len++] = (seq_t)next_scan;  /* cast fits into seq_t */
        }
        ++next_scan;
    }
    return cache[n - 1];
}
