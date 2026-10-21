/*
 * A246395  Nonnegative integers k satisfying cos(k) >= 0 and cos(k+1) >= 0.
 * https://oeis.org/A246395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth non‑negative integer k such that cos(k) >= 0 and
 *            cos(k+1) >= 0. Computed by brute‑force scanning of integers
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (cache_len <= (size_t)n) {
        idx_t k = (cache_len == 0) ? 0 : ((idx_t)cache[cache_len - 1] + 1);
        while (cache_len <= (size_t)n) {
            double ck  = cos((double)k);
            double ck1 = cos((double)(k + 1));
            if (ck >= 0.0 && ck1 >= 0.0) {
                grow_cache(cache_len + 1);
                cache[cache_len] = (seq_t)k;
                cache_len++;
            }
            k++;
        }
    }
    return cache[(size_t)n];
}
