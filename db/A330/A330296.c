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
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   ctz_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ctz_u64.h"
#include "../utils/mem.h"

static bool valid_bii(uint64_t x)
{
    if (__builtin_popcountll(x) < 2)  /* need at least two blocks */
        return false;

    uint64_t used = 0;          /* used bit positions of binary indices */
    uint64_t temp = x;
    while (temp) {
        uint64_t lsb = temp & -temp;
        int pos = ctz_u64(lsb) + 1;  /* binary index (1‑based) */
        uint64_t idx = (uint64_t)pos;
        uint64_t t2 = idx;
        while (t2) {
            uint64_t lsb2 = t2 & -t2;
            int pos2 = ctz_u64(lsb2) + 1;  /* position inside idx */
            uint64_t mask = 1ULL << (pos2 - 1);
            if (used & mask)
                return false;               /* overlap */
            used |= mask;
            t2 ^= lsb2;
        }
        temp ^= lsb;
    }
    return true;
}

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
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return (seq_t)0; /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)candidate;
        }
    }
    return cache[n - 1];
}
