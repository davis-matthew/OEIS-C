/*
 * A351893  Numbers that contain only even digits in their factorial-base
 *          representation.
 * https://oeis.org/A351893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 576, u32 n <= 14400, u64 n <= 14400,
 *            u128 n <= 14400, big any n
 * Validated: standard, pass, 68 terms through n = 9318 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer whose factorial‑base representation contains only even digits (OEIS A351893). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper that tests whether the factorial‑base digits of x are all even */
static bool factor_base_all_even(seq_t x) {
    if (x == 0) return true;              /* 0 is treated as a valid term */
    for (seq_t k = 1; x > 0; ++k) {
        seq_t digit = x % (k + 1);
        if ((digit & 1) != 0) return false; /* odd digit found */
        x /= (k + 1);
    }
    return true;
}

/* Static cache holding computed terms */
static seq_t *cache   = NULL;      /* array of terms */
static size_t cap     = 0;         /* current capacity of cache */
static size_t len     = 0;         /* number of terms stored */
static seq_t last_chk = 0;         /* last integer that has been examined */

/* Main sequence function */
seq_t a(idx_t n) {
    /* Ensure the cache contains at least n terms */
    while (len < n) {
        if (factor_base_all_even(last_chk)) {
            if (len == cap) {
                cap = cap ? 2 * cap : 128;
                /* Grow the cache; cast to the correct pointer type */
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[len++] = last_chk;
        }
        ++last_chk;
    }
    /* The sequence is 1-indexed: a(1) = 0 */
    return cache[n - 1];
}
