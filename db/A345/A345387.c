/*
 * A345387  Numbers k such that k, 2k, 3k, 4k all contain digit 1.
 * https://oeis.org/A345387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   contains_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the n-th positive integer k such that
   k, 2k, 3k and 4k all contain at least one decimal digit '1'.
   The sequence has offset 1 (a(1)=531).  The function uses a
   simple brute‑force search with memoisation so that repeated calls
   with arbitrary values of n are answered in O(1) time after the
   needed terms have been found. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_one.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // offset starts at 1; undefined for n=0

    /* static state keeps results across multiple calls */
    static seq_t  *cache   = NULL; // dynamic array of found k's
    static size_t  cache_len = 0;  // number of elements currently stored
    static size_t  cache_cap = 0;  // allocated capacity
    static idx_t   next_k    = 1;  // start scanning from 1

    /* Compute additional terms until we have at least n of them */
    while (cache_len < n) {
        /* Find the next k that satisfies the property */
        while (true) {
            seq_t k = (seq_t)next_k;

            if (contains_one(k) &&
                contains_one(k * (seq_t)2) &&
                contains_one(k * (seq_t)3) &&
                contains_one(k * (seq_t)4)) {

                /* Ensure the cache has space; double capacity when needed */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;          // allocation failure
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                ++next_k;                     // move to the next integer for future searches
                break;                        // found a valid term
            }
            ++next_k;                         // test next integer
        }
    }

    return cache[n - 1];   // return the n-th element (1‑based offset)
}
