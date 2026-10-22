/*
 * A326911  BII-numbers of set-systems with empty intersection.
 * https://oeis.org/A326911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer (starting from n = 1) whose set-system,
 *            defined by taking the binary indices of each binary index of the
 *            number, has empty intersection. Computation: For a candidate
 *            number x, let I be the set of positions (1‑based) of 1‑bits in
 *            the reversed binary expansion of x. For each...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   empty_intersection_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/empty_intersection_u64.h"

/* public routine required by the build */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache   = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;
    static uint64_t next_candidate = 0;   /* next integer to test */

    if (n == 0)                 /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* find next qualifying number */
        uint64_t candidate = next_candidate;
        while (!empty_intersection_u64(candidate))
            ++candidate;

        /* append to cache, resizing if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);             /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)candidate;

        next_candidate = candidate + 1;    /* continue from the next integer */
    }

    return cache[n - 1];    /* a(1) is the first term stored */
}
