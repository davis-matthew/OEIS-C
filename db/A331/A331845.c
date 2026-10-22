/*
 * A331845  Number of compositions (ordered partitions) of n into distinct
 *          cubes.
 * https://oeis.org/A331845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 440, u16 n <= 2024, u32 n <= 8280, u64 n <= 9833,
 *            u128 n <= 9833, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compositions into distinct cubes:
   a(n) = Σ_{S⊂{1³,2³,…,⌊n^{1/3}⌋³}} |S|!  if ΣS = n */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_cube_root.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cached values up to cached_up_to */
    static idx_t cached_up_to = 0;

    /* Recompute cache if n is larger than what we have cached */
    if (!cache || n > cached_up_to) {
        /* Allocate a new cache of size (n+1) */
        size_t cache_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* out of memory → zero */

        /* Determine the number of distinct cube values needed */
        unsigned int cube_count = (unsigned int)int_cube_root(n);
        /* Precompute the cube values and their factorials */
        uint64_t cubes[22];
        seq_t fact[22];
        fact[0] = (seq_t)1;
        for (unsigned int i = 1; i <= cube_count; ++i) {
            uint64_t val = (uint64_t)i * (uint64_t)i * (uint64_t)i;
            cubes[i - 1] = val;
            fact[i] = fact[i - 1] * (seq_t)i;
        }

        /* Enumerate all non‑empty subsets of the available cubes */
        size_t limit = 1ULL << cube_count;   /* 2^cube_count */
        for (size_t mask = 1; mask < limit; ++mask) {
            uint64_t sum = 0;
            unsigned int sz = 0;
            size_t m = mask;
            while (m) {
                unsigned int bit = __builtin_ctzll(m);
                m &= m - 1;                  /* clear lowest set bit */
                sum += cubes[bit];
                ++sz;
                if (sum > (uint64_t)n) break; /* prune */
            }
            if (sum <= (uint64_t)n)
                new_cache[(size_t)sum] += fact[sz];
        }

        /* The empty composition contributes 1 for n = 0 */
        new_cache[0] = (seq_t)1;

        /* Replace the old cache */
        free(cache);
        cache = new_cache;
        cached_up_to = n;
    }

    return cache[n];
}
