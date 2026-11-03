/*
 * A323545  a(n) = Product_{k=0..n} (k^7 + (n-k)^7).
 * https://oeis.org/A323545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^7 + (n-k)^7)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 79 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_u64_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
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

static seq_t *pow7 = NULL;
static size_t pow7_cap = 0;
static idx_t pow7_len = 0;

static void ensure_pow7(idx_t n) {
    if (n < pow7_len) return;
    size_t new_cap = (size_t)n + 1;
    if (pow7 == NULL) {
        pow7 = seq_alloc(new_cap);
    } else if (new_cap > pow7_cap) {
        pow7 = seq_realloc(pow7, pow7_cap, new_cap);
    }
    for (idx_t i = pow7_len; i <= n; ++i) {
        pow7[i] = ipow_u64_idx((uint64_t)i, (idx_t)7);
    }
    pow7_cap = new_cap;
    pow7_len = new_cap;
}

seq_t a(idx_t n) {
    ensure_pow7(n);
    seq_t product = (seq_t)1;
    for (idx_t k = (idx_t)0; k <= n; ++k) {
        seq_t term = pow7[k] + pow7[n - k];
        product = seq_mul(product, term);
    }
    return product;
}
