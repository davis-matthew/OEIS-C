/*
 * A330296  BII-numbers of set partitions with at least two blocks.
 * https://oeis.org/A330296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer n ≥ 1 with at least two 1‐bits in binary and
 *            such that the sets of 1‐bit positions of every binary index of n
 *            are pairwise disjoint
 * Range:     u8 n <= 21, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   ctz_uint64, valid_bii
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ctz_uint64.h"
#include "../utils/valid_bii.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t next_test   = 1;   /* next integer to examine */

    while (cache_size < (size_t)n) {
        idx_t candidate = next_test++;
        if (valid_bii((uint64_t)candidate)) {
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)candidate;
        }
    }
    return cache[n - 1];
}
