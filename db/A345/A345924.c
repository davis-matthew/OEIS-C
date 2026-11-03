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
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Check whether the k‑th composition has alternating sum -2. */
static bool alt_sum_minus2(idx_t k)
{
    if (k == 0) return false;          /* composition of 0 is empty */
    unsigned long long kk = (unsigned long long)k;
    int m = __builtin_popcountll(kk);   /* number of parts */
    int sign = (m & 1) ? 1 : -1;         /* (-1)^(m-1) */
    long long alt = 0;
    idx_t pos = 1;                      /* LSB is position 1 */
    idx_t prev_pos = 0;
    unsigned long long temp = kk;
    while (temp) {
        if (temp & 1) {
            long long d = (long long)pos - (long long)prev_pos;
            alt += (long long)sign * d;
            prev_pos = pos;
            sign = -sign;
        }
        temp >>= 1;
        pos++;
    }
    return alt == -2;
}

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
        cache = mem_new(seq_t, cache_cap);
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
                cache = mem_resize(seq_t, cache, cache_cap);
                if (!cache) abort();
            }
            cache[cache_size++] = (seq_t)last_k_checked;
        }
    }
    return cache[n - 1];
}
