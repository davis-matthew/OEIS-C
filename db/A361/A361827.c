/*
 * A361827  For any number k >= 0, let T_k be the triangle whose base
 *          corresponds to the ternary expansion of k (without leading zeros)
 *          and other values, say t above u and v, satisfy t = (-u-v) mod 3;
 *          this sequence lists the numbers k such that the configurations of
 *          0's, 1's and 2's in T_k are the same up to...
 * https://oeis.org/A361827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*(n+1)/2
 * Range:     u8 n <= 20, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   candidate_is_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A361827.  For each k we construct the
   triangle T_k and test whether every orbit of the 120° rotation
   contains one of each symbol 0,1,2; i.e., the configuration is
   the same up to a permutation of the symbols.  The sequence is
   generated incrementally and cached for random accesses. */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/candidate_is_valid.h"

/* Static cache for generated sequence values. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_candidate = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n=0; harmless */

    while (cache_len < (size_t)n) {
        seq_t k = next_candidate;
        while (!candidate_is_valid(k)) ++k;   /* find next valid term */

        /* Append to cache. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) return (seq_t)0;  /* allocation failure */
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_len++] = k;
        next_candidate = k + (seq_t)1;
    }
    return cache[n - 1];
}
