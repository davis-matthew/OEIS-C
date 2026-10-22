/*
 * A347180  Lexicographically earliest sequence of distinct positive integers
 *          such that the first digit of a(n) is visible in a(n) * a(n+1).
 * https://oeis.org/A347180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A347180
   Lexicographically earliest sequence of distinct positive integers such
   that the first digit of a(n) is visible in a(n) * a(n+1).
   Computed iteratively with memoisation.  */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/digit_counts.h"

/* The sequence is generated iteratively: given a(n-1), find the smallest
   unused integer a(n) for which the first digit of a(n-1) appears somewhere
   in the decimal representation of a(n-1)*a(n).  All terms are stored
   in a growable cache so that calls with any order of indices work.  */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len  = 0;   /* number of terms already stored   */

    /* Table of which integers have already been used.  The index
       corresponds to the integer value.  */
    static uint8_t *used = NULL;
    static size_t used_cap = 0;     /* length of used array               */

    /* If we have already computed up to n, just return the cached
       value.  Otherwise grow the cache up to n.  */
    if (n > cache_len) {
        /* First call: allocate a tiny cache and initialise a(1)=1. */
        if (cache_len == 0) {
            cache_cap = 8;
            cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
            if (!cache) abort();          /* no memory -> fatal */

            /* Allocate the used table big enough for index 1. */
            used_cap = 2;
            used = (uint8_t *)malloc(sizeof(uint8_t) * used_cap);
            if (!used) abort();
            memset(used, 0, used_cap);
            used[1] = 1;                  /* a(1)=1 has been used */
            cache[0] = (seq_t)1;
            cache_len = 1;
        }

        /* Grow the cache and compute new terms one by one. */
        for (; cache_len < n; ++cache_len) {
            seq_t prev = cache[cache_len - 1];

            /* Determine the first digit of prev. */
            seq_t tmp = prev;
            while (tmp >= (seq_t)10)
                tmp /= (seq_t)10;
            unsigned char first_digit = (unsigned char)tmp; /* 1‑9 */

            /* Find the smallest unused integer whose product with prev
               contains the first digit.  */
            uint64_t cand = 1;
            seq_t term;
            for (;;) {
                /* Ensure we have storage for cand. */
                if ((size_t)cand >= used_cap) {
                    size_t new_used_cap = used_cap ? used_cap * 2 : 8;
                    while (new_used_cap <= (size_t)cand)
                        new_used_cap <<= 1;
                    uint8_t *tmp_used =
                        (uint8_t *)realloc(used, new_used_cap * sizeof(uint8_t));
                    if (!tmp_used) abort();
                    /* Zero the newly allocated slots. */
                    memset(tmp_used + used_cap, 0,
                           new_used_cap - used_cap);
                    used = tmp_used;
                    used_cap = new_used_cap;
                }

                if (!used[(size_t)cand]) {
                    seq_t prod = prev * (seq_t)cand;
                    unsigned char freq[10] = {0};
                    digit_counts(prod, freq);
                    if (freq[first_digit]) {
                        term = (seq_t)cand;
                        used[(size_t)cand] = 1;
                        break;
                    }
                }
                ++cand;
            }

            /* Ensure the cache has room for the new term. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp_cache =
                    (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp_cache) abort();
                cache = tmp_cache;
                cache_cap = new_cap;
            }
            cache[cache_len] = term;
        }
    }

    return cache[n - 1];
}
