/*
 * A344214  Numbers k such that repeated iterations of f(m) =
 *          (digsum(f(m-1)))^2 + 1 starting from f(1) = k will eventually
 *          yield 5 before any other single-digit number.
 * https://oeis.org/A344214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A344214: numbers k such that repeated iterations of
   f(m) = (digit sum of m)^2 + 1 starting from m = k eventually
   reach 5 before any other single‑digit number. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Computes whether a given starting value k reaches a single‑digit 5
   before any other single‑digit (or enters the known 65‑122‑26 cycle). */
static bool leads_to_five(seq_t k) {
    seq_t m = k;
    /* We iterate until we meet a single digit or detect the
       65–122–26 cycle. A safety iteration cap prevents infinite loops. */
    for (int step = 0; step < 1000; ++step) {
        if (m < 10)                    /* reached a single digit */
            return m == 5;             /* success only if it is 5 */
        if (m == 65 || m == 122 || m == 26)
            return false;             /* will loop forever elsewhere */

        /* compute digsum(m) */
        seq_t s = 0;
        seq_t t = m;
        while (t > 0) {
            s += t % 10;
            t /= 10;
        }
        m = s * s + 1;
    }
    return false;                     /* fallback – should not happen */
}

/* cache of found sequence values */
static seq_t *cache = NULL;
static idx_t cached_len = 0;
static idx_t cached_cap = 0;
static idx_t next_candidate = 1;

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined, but safe guard */

    /* Extend cache until we have n terms */
    while (cached_len < n) {
        seq_t val = (seq_t)next_candidate;
        if (leads_to_five(val)) {
            if (cached_len == cached_cap) {
                size_t new_cap = cached_cap ? cached_cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0;                 /* allocation failed */
                cache = tmp;
                cached_cap = new_cap;
            }
            cache[cached_len++] = val;
        }
        ++next_candidate;
    }
    return cache[n - 1];               /* 1‑based indexing */
}
