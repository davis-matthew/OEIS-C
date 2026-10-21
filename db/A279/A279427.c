/*
 * A279427  Numbers k such that k^2 has an odd number of digits and the middle
 *          digit is 7.
 * https://oeis.org/A279427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 2511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Determine the n-th integer k such that k² has an odd number of decimal
   digits and the middle digit of that square is 7.
   There is no closed form; we compute by direct search and cache the
   results.  The function is order‑independent and fast enough for n up
   to at least 10 000. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/seq_realloc.h"

static bool is_prop(seq_t k) {
    seq_t sq = k * k;                       /* k² */
    int d = digit_len(sq);                  /* number of decimal digits */
    if ((d & 1) == 0) return false;         /* even number of digits */
    seq_t pow_half = pow10((idx_t)(d / 2));  /* 10^(floor(d/2)) */
    /* extract the middle digit */
    int mid = (int)((sq / pow_half) % (seq_t)10);
    return mid == 7;
}

/* cache of known terms a(1), a(2), … */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_k = (seq_t)1;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety for n < 1 */
    size_t need = (size_t)n;
    while (cache_len < need) {
        if (is_prop(next_k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();        /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_k;
        }
        ++next_k;
    }
    return cache[need - 1];
}
