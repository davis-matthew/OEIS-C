/*
 * A345924  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has alternating sum -2.
 * https://oeis.org/A345924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k whose k‑th composition (in standard
 *            order) has alternating sum -2. The function searches for such k
 *            by incrementing k and testing the alternating sum of its
 *            composition
 * Range:     u8 n <= 27, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   alt_sum_minus2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/alt_sum_minus2.h"

/* Cache of found values to allow non‑sequential calls. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static idx_t last_k_checked = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort(); /* allocation failure */
        cache_size = 0;
        last_k_checked = 0;
        initialized = true;
    }
    /* Extend the cache until it contains at least n elements (n is 1‑based). */
    while (cache_size < (size_t)n) {
        last_k_checked++;
        if (alt_sum_minus2(last_k_checked)) {
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();
            }
            cache[cache_size++] = (seq_t)last_k_checked;
        }
    }
    return cache[n - 1];
}
