/*
 * A340131  Numbers whose ternary expansions have the same number of 1's and
 *          2's and, in each prefix (initial fragment), at least as many 1's
 *          as 2's.
 * https://oeis.org/A340131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   good_ternary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340131: numbers whose ternary expansions contain the same
   number of 1's and 2's and in every prefix have at least as many 1's
   as 2's. The function builds the sequence by brute‑force search,
   memoising results between calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/good_ternary.h"

/* Helper to allocate an array of seq_t with correct constructor semantics
   in both C and C++ builds. */
static void *seq_alloc(size_t n) {
#ifdef __cplusplus
    return static_cast<seq_t*>(new seq_t[n]);
#else
    return (void*)malloc(n * sizeof(seq_t));
#endif
}

static void seq_free(void *p) {
#ifdef __cplusplus
    delete[] static_cast<seq_t*>(p);
#else
    free(p);
#endif
}

/* Dynamic array to store computed sequence values. */
static seq_t *seq_cache = NULL;
static idx_t seq_cnt   = 0;       /* number of values stored    */
static idx_t seq_cap   = 0;       /* allocated capacity         */
static seq_t next_candidate = 0;  /* next integer to test       */

/* Grow the cache to at least 'needed' capacity. */
static void seq_grow(idx_t needed) {
    if (seq_cap >= needed) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;

    seq_t *new_arr = (seq_t*)seq_alloc(new_cap);
    for (idx_t i = 0; i < seq_cnt; ++i) new_arr[i] = seq_cache[i];
    if (seq_cache) seq_free(seq_cache);
    seq_cache = new_arr;
    seq_cap = new_cap;
}

/* Public sequence function requested by the framework. */
seq_t a(idx_t n) {
    /* Ensure we have at least 'n' values cached. */
    while (seq_cnt < n) {
        seq_grow(seq_cnt + 1);
        if (good_ternary((seq_t)next_candidate)) {
            seq_cache[seq_cnt++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq_cache[n - 1];  /* 1‑based indexing */
}
