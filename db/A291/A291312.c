/*
 * A291312  Numbers that contain exactly one pair of identical digits x and a
 *          triple of identical digits y (x not equal y).
 * https://oeis.org/A291312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   has_pair_and_triple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches consecutive integers for numbers that contain
   exactly one pair of identical digits and exactly one triple of identical
   digits (x != y).  It keeps a static cache of all results found so far to
   support arbitrary call order. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_pair_and_triple.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* cache of results found so far */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static uint64_t next_val = 10001;   /* first integer to test */

    /* Grow the cache when necessary */
    if (len < (size_t)n) {
        if (cap == 0) {
            cap = 16;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
            if (!cache) abort();
        }
        while (len < (size_t)n) {
            if (has_pair_and_triple(next_val)) {
                if (len == cap) {
                    size_t new_cap = cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cap = new_cap;
                }
                cache[len++] = (seq_t)next_val;
            }
            next_val++;   /* advance to the next candidate */
        }
    }

    return cache[n - 1];
}
