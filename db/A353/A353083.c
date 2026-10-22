/*
 * A353083  The second column of the Trithoff (tribonacci) array.
 * https://oeis.org/A353083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the nth positive integer whose tribonacci representation ends in 10. */

#include <stdlib.h>

/* Dynamic array of tribonacci numbers (T1 = 1, T2 = 2, T3 = 4, ...). */
static seq_t *tribs   = NULL;
static size_t trib_cap = 0;
static size_t trib_len = 0;

/* Make sure tribs contains all tribonacci numbers ≤ max. */
static void ensure_tribs_leq(seq_t max) {
    if (trib_len == 0) {
        trib_cap = 4;
        tribs = (seq_t *)malloc(trib_cap * sizeof(seq_t));
        tribs[0] = 1;          /* T1 */
        tribs[1] = 2;          /* T2 */
        tribs[2] = 4;          /* T3 */
        trib_len = 3;
    }
    while (trib_len > 0 && tribs[trib_len - 1] <= max) {
        if (trib_len == trib_cap) {
            trib_cap *= 2;
            tribs = (seq_t *)realloc(tribs, trib_cap * sizeof(seq_t));
        }
        tribs[trib_len] =
            tribs[trib_len - 1] + tribs[trib_len - 2] + tribs[trib_len - 3];
        trib_len++;
    }
}

/* Return 1 if val has a tribonacci representation ending with binary 10. */
static int has_ending10(seq_t val) {
    ensure_tribs_leq(val);
    size_t idx = trib_len - 1;
    while (idx > 0 && tribs[idx] > val) --idx;

    seq_t rem = val;
    int bit1 = 0, bit0 = 0;

    for (size_t i = idx; i > 0; --i) {
        if (tribs[i] <= rem) {
            rem -= tribs[i];
            if (i == 1) bit1 = 1;
        }
    }
    if (tribs[0] <= rem) {
        rem -= tribs[0];
        bit0 = 1;
    }
    return bit0 == 0 && bit1 == 1;
}

/* Cache of computed sequence values. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_size = 0;
static seq_t last_checked = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety: offset is 1 */
    while (cache_size < n) {
        ++last_checked;
        if (has_ending10(last_checked)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = last_checked;
        }
    }
    return cache[n - 1];
}
