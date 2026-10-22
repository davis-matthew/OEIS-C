/*
 * A351893  Numbers that contain only even digits in their factorial-base
 *          representation.
 * https://oeis.org/A351893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 576, u32 n <= 9318, u64 n <= 9318,
 *            u128 n <= 9318, big any n
 * Helpers:   factor_base_all_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer whose factorial‑base representation contains only even digits (OEIS A351893). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_base_all_even.h"

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
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = last_chk;
        }
        ++last_chk;
    }
    /* The sequence is 1-indexed: a(1) = 0 */
    return cache[n - 1];
}
