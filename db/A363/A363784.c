/*
 * A363784  L.g.f.: log( Sum_{k>=0} x^(k^4) ).
 * https://oeis.org/A363784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 39, i16 n <= 87, i32 n <= 191, i64 n <= 390,
 *            i128 n <= 801, big any n
 * Validated: standard, pass, 78 terms through n = 966 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A363784 : a(n) = n*(1 if n is a fourth power else 0) - Σ a(n-m) over all fourth powers m < n */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* check whether n is a perfect fourth power */
static bool is_fourth_power(idx_t n) {
    if (n == 0) return true; /* 0^4 = 0, but a(0) is never queried */
    for (idx_t k = 1;; ++k) {
        seq_t fp = (seq_t)k * k * k * k;   /* k^4 */
        if (fp == n) return true;
        if (fp > n)  return false;
    }
}

/* external linkage */
static seq_t *a_cache = NULL;   /* cached a[0..cap] (index 0 unused) */
static idx_t cache_cap = 0;     /* maximum n for which a_cache is valid */

/* extend cached values up to new_cap (inclusive) */
static void up_to(idx_t new_cap) {
    /* allocate or grow the cache array */
    a_cache = mem_resize(seq_t, a_cache, (new_cap + 1));
    if (!a_cache) { abort(); } /* out‑of‑memory */

    /* compute new values, from cache_cap+1 up to new_cap */
    for (idx_t n = cache_cap + 1; n <= new_cap; ++n) {
        seq_t sum = 0;
        /* sum over all fourth powers m < n of a[n-m] */
        for (idx_t k = 1;; ++k) {
            seq_t fp = (seq_t)k * k * k * k; /* m = k^4 */
            if (fp >= n) break;             /* only m < n are needed */
            sum += a_cache[n - (idx_t)fp];
        }
        seq_t term = is_fourth_power(n) ? (seq_t)n : (seq_t)0;
        a_cache[n] = term - sum;
    }
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n < 1, return 0 */
    if (n > cache_cap) up_to(n);
    return a_cache[n];
}
