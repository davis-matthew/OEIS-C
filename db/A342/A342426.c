/*
 * A342426  Niven numbers in base 3/2: numbers divisible by their sum of
 *          digits in fractional base 3/2 (A244040).
 * https://oeis.org/A342426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 2933, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   base32_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A342426 – Niven numbers in base 3/2.
   The algorithm generates successive integers, sums their
   digits in base 3/2 using the rule d = m mod 3 and
   next = ((m-d)/3)*2, and keeps those that are divisible
   by this sum. Cached results allow fast repeated calls. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/base32_digit_sum.h"

static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* capacity of array */
static idx_t    next_candidate = 1; /* next integer to test */

/* Expand the cache to accommodate at least 'needed' elements.  */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but safe */
    /* Extend cached sequence until we have at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t v = (seq_t)next_candidate;          /* candidate value */
        seq_t s = base32_digit_sum(v);           /* digit sum                */
        if (s && (v % s) == 0) {                 /* divisible?                */
            grow_cache(cache_len + 1);
            cache[cache_len++] = v;
        }
        ++next_candidate;                          /* next integer to test     */
    }
    return cache[n - 1];
}
