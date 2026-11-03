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
 * Validated: standard, pass, 62 terms through n = 9969 (2026-11-01)
 * Helpers:   num_digits
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
#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static bool is_prop(seq_t k) {
    seq_t sq = k * k;                       /* k² */
    int d = num_digits(sq);                  /* number of decimal digits */
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
