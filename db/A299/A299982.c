/*
 * A299982  Lexicographic first sequence of nonnegative integers such that
 *          a(n) + a(n+1) has a digit 2, and no term occurs twice.
 * https://oeis.org/A299982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   has_digit_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived by greedy construction: 
   a(0) = 0; for n > 0, a(n) = the smallest unused non‑negative integer m
   such that a(n‑1) + m contains the digit 2 in base 10. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/has_digit_two.h"

seq_t a(idx_t n)
{
    /* Persistent state across calls */
    static seq_t *cache = NULL;      /* array of computed terms          */
    static size_t cache_len = 0;     /* number of stored terms           */
    static size_t cache_cap = 0;     /* allocated capacity for cache     */

    static uint8_t *used = NULL;     /* used[k] == 1 iff integer k used */
    static size_t used_size = 0;     /* allocated size for used array    */

    /* Initialize the cache on first call */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }
    /* Grow the cache array if necessary */
    if (n >= cache_cap) {
        while (cache_cap <= (size_t)n) {
            cache_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
    }

    /* Generate terms up to index n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0) {
            /* The first term is always 0 */
            val = (seq_t)0;
            if (used_size == 0) {
                used_size = 1;
                used = (uint8_t *)calloc(used_size, sizeof(uint8_t));
                if (!used) exit(1);
            }
            used[0] = 1;
        } else {
            seq_t prev = cache[i - 1];
            size_t j;
            for (j = 0; ; ++j) {
                if (j >= used_size) {
                    size_t new_size = j + 1;
                    uint8_t *tmp = (uint8_t *)realloc(used, new_size);
                    if (!tmp) exit(1);
                    memset(tmp + used_size, 0, new_size - used_size);
                    used = tmp;
                    used_size = new_size;
                }
                if (!used[j]) {
                    seq_t sum = prev + (seq_t)j;
                    if (has_digit_two(sum)) {
                        val = (seq_t)j;
                        used[j] = 1;
                        break;
                    }
                }
            }
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
