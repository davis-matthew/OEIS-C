/*
 * A300476  Numbers the square of which can be written as a sum of four
 *          nonzero bi-quadratics.
 * https://oeis.org/A300476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   cmp_seq_custom, copy_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A300476 – smallest integers w such that
   w² = x⁴ + y⁴ + z⁴ + t⁴   for some positive integers x,y,z,t. */
/* The function a(idx_t n) returns the n‑th such integer (n≥1).  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/cmp_seq_custom.h"
#include "../utils/copy_seq.h"

/* ------------------------------------------------------------------ */
/* Auxiliary routines for memory handling that work in both C11 and C++17. */
static seq_t* alloc_seq(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return malloc(cap * sizeof(seq_t));
#endif
}
static void free_seq(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* ------------------------------------------------------------------ */
/* Integer square root for idx_t.  Works for 64‑bit unsigned integers. */
static idx_t isqrt_idx(idx_t n) {
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Global tables used in the search. */
static seq_t *pow4 = NULL;          /* pow4[i] = i⁴, i = 0 .. pow4_limit */
static idx_t pow4_limit = 0;

static seq_t *pairSums = NULL;      /* All distinct sums x⁴ + y⁴  (1 ≤ x ≤ y ≤ pair_limit) */
static size_t pair_len = 0;         /* Number of elements in pairSums */
static idx_t pair_limit = 0;        /* Largest i used to build pairSums (equals pow4_limit after rebuild) */

static seq_t *seq_cache = NULL;     /* Sequence values found so far */
static size_t seq_cap = 0;          /* Capacity of seq_cache */
static size_t seq_len = 0;          /* Number of elements stored */
static idx_t next_w = 2;            /* Next candidate w to test */

/* ------------------------------------------------------------------ */
/* Ensure pow4 has indices up to need; rebuild if necessary. */
static void ensure_pow4_limit(idx_t need) {
    if (need <= pow4_limit) return;
    idx_t new_limit = pow4_limit ? pow4_limit * 2 : 1;
    while (new_limit < need) new_limit = new_limit * 2 + 1;

    seq_t *new_pow4 = alloc_seq(new_limit + 1);
    if (pow4) {
        copy_seq(new_pow4, pow4, pow4_limit + 1);
    }
    /* compute new entries */
    for (idx_t i = pow4_limit + 1; i <= new_limit; ++i) {
        seq_t v = (seq_t)i;
        new_pow4[i] = v * v * v * v;
    }
    if (pow4) free_seq(pow4);
    pow4 = new_pow4;
    pow4_limit = new_limit;
}

/* ------------------------------------------------------------------ */
/* Rebuild pairSums when pow4_limit has increased. */
static void recompute_pair_sums_if_needed(void) {
    if (pair_limit == pow4_limit) return;   /* nothing to do */
    pair_limit = pow4_limit;
    const size_t sz = (size_t)pair_limit * (pair_limit + 1) / 2;
    seq_t *new_ps = alloc_seq(sz);
    size_t idx = 0;
    for (idx_t i = 1; i <= pair_limit; ++i) {
        seq_t pi = pow4[i];
        for (idx_t j = i; j <= pair_limit; ++j) {
            new_ps[idx++] = pi + pow4[j];
        }
    }
    pair_len = idx;

    qsort(new_ps, pair_len, sizeof(seq_t), cmp_seq_custom);

    /* remove duplicates: one pass over sorted array */
    size_t uniq = 0;
    for (size_t i = 0; i < pair_len; ++i) {
        if (uniq == 0 || new_ps[i] != new_ps[uniq - 1]) {
            new_ps[uniq++] = new_ps[i];
        }
    }
    pair_len = uniq;

    if (pairSums) free_seq(pairSums);
    pairSums = new_ps;
}

/* ------------------------------------------------------------------ */
/* Test whether a given w has the required representation. */
static bool has_representation(idx_t w) {
    seq_t w2 = (seq_t)w * (seq_t)w;
    const idx_t t_max = isqrt_idx(w);            /* t ≤ sqrt(w)   */
    for (idx_t t = 1; t <= t_max; ++t) {
        seq_t t4 = pow4[t];
        if (t4 >= w2) break;
        seq_t rem = w2 - t4;
        for (idx_t z = 1; z <= t_max; ++z) {
            seq_t z4 = pow4[z];
            if (z4 >= rem) break;
            seq_t need = rem - z4;
            if (bsearch(&need, pairSums, pair_len, sizeof(seq_t), cmp_seq_custom)) {
                return true;
            }
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Main entry point for the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but avoid UB */

    while (seq_len < n) {
        const idx_t t_needed = isqrt_idx(next_w);
        ensure_pow4_limit(t_needed);
        recompute_pair_sums_if_needed();

        if (has_representation(next_w)) {
            if (seq_len == seq_cap) {      /* grow cache */
                size_t new_cap = seq_cap ? seq_cap * 2 : 8;
                seq_t *new_cache = alloc_seq(new_cap);
                if (seq_cache) {
                    copy_seq(new_cache, seq_cache, seq_len);
                    free_seq(seq_cache);
                }
                seq_cache = new_cache;
                seq_cap = new_cap;
            }
            seq_cache[seq_len++] = (seq_t)next_w;
        }
        ++next_w;
    }
    return seq_cache[n - 1];
}
