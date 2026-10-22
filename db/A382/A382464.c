/*
 * A382464  Positive integers that contain an even digit d immediately
 *          preceded by a digit >= d.
 * https://oeis.org/A382464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_even_preceded_by_ge
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th positive integer that contains an even digit d
   immediately preceded by a digit p >= d. The function scans
   successive integers, retains those that satisfy the pattern,
   and returns the n-th retained number. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_even_preceded_by_ge.h"

/* Main sequence function.  Uses a static dynamic array to cache
   previously computed terms, so calls in any order are efficient. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of found terms      */
    static idx_t cache_cap = 0;   /* capacity of cache        */
    static idx_t cache_size = 0;  /* number of terms cached   */
    static idx_t next_candidate = 1;   /* next integer to test    */

    /* Ensure cache storage exists. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Grow cache until we have at least n terms. */
    while (cache_size < n) {
        /* Find next candidate that satisfies the property. */
        while (true) {
            seq_t val = (seq_t)next_candidate;
            if (has_even_preceded_by_ge(val)) {
                /* Store it. */
                if (cache_size == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                }
                cache[cache_size++] = val;
                next_candidate++;   /* advance to next integer for next search */
                break;
            }
            next_candidate++;       /* check following integer */
        }
    }

    /* Return the requested term. */
    return cache[n - 1];
}
