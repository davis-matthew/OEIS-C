/*
 * A382623  Positive integers that contain an even digit d immediately
 *          preceded by a digit <= d.
 * https://oeis.org/A382623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th positive integer containing an even digit d
 *            immediately preceded by a digit <= d
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_even_preceded
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_even_preceded.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;            /* cached terms of the sequence */
    static size_t len = 0;                 /* number of cached terms */
    static size_t cap = 0;                 /* capacity of the cache */
    static seq_t next_candidate = 1;       /* next integer to test */

    /* Initialise cache on first call */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Generate terms until we have reached the desired index */
    while (len < (size_t)n) {
        if (has_even_preceded(next_candidate)) {
            if (len >= cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
