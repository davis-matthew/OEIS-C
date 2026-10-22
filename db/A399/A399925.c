/*
 * A399925  Numbers k such that A007429(k) is even.
 * https://oeis.org/A399925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   k_is_target
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A399925: numbers k such that either v₂(k) is odd or
   v_p(k) ≡ 2 or 3 (mod 4) for some odd prime p.  The function
   generates terms by testing consecutive integers and caches the
   found ones. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/k_is_target.h"

/* cache for already found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t  next_k   = 1;   /* next integer to test */

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    while (cache_len < n) {
        ++next_k;
        if (k_is_target(next_k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
    }
    return cache[n - 1];
}
