/*
 * A279410  Numbers whose squares have identical middle digits in base 10.
 * https://oeis.org/A279410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose square has an even number
 *            of digits and whose two middle digits are equal. Computed by a
 *            simple incremental search
 * Range:     u8 none, u16 n <= 7, u32 n <= 4352, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 8843 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
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

static bool has_identical_middle_digits(seq_t sq) {
    int len = num_digits(sq);
    if (len & 1) return false;          /* odds are ignored */
    unsigned int k = (len / 2) - 1;     /* exponent of 10^k that shifts the middle two digits out */
    seq_t powk = pow10((idx_t)k);       /* 10^k as seq_t */
    seq_t middle = (sq / powk) % (seq_t)100; /* the two middle digits */
    return ((middle / 10) % 10) == (middle % 10);
}

seq_t a(idx_t n) {
    /* Lazy cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_checked = 0;

    if (n == 0) return 0; /* undefined for 0 but harmless */

    while (cache_len < n) {
        seq_t cand = last_checked + 1;
        seq_t square = cand * cand;
        if (has_identical_middle_digits(square)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        last_checked = cand;
    }
    return cache[n - 1];
}
