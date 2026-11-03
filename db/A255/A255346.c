/*
 * A255346  Numbers k such that k and k+1 both have at least two distinct
 *          prime factors.
 * https://oeis.org/A255346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 80 terms through n = 924 (2026-11-03)
 * Helpers:   mem, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A255346: numbers k such that k and k+1 both have at least two distinct prime factors */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for n=0, but safe fallback */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cached_count = 0;
    static uint64_t last_checked = 1;  /* next k to test */

    /* ensure cache can hold at least n elements */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) exit(EXIT_FAILURE); /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if we already have the term, return it */
    if (n <= cached_count) return cache[n - 1];

    /* continue searching from the last checked number */
    uint64_t k = last_checked;
    while (cached_count < n) {
        if (omega(k) >= 2 && omega(k + 1) >= 2) {
            cache[cached_count] = (seq_t)k;
            cached_count++;
        }
        k++;
    }
    last_checked = k;  /* next start point */
    return cache[n - 1];
}
