/*
 * A345167  Numbers k such that the k-th composition in standard order is
 *          alternating.
 * https://oeis.org/A345167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 3121, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A345167 sequence by iterating the binary indices
   of compositions in standard order and checking for alternation. */

#include <stdlib.h>
#include <stdbool.h>

/* Stylistic helper: ensures capacity for a dynamic array */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size) {
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : *cap * 2;
    while (new_cap < needed) new_cap *= 2;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) exit(EXIT_FAILURE);
    *ptr = tmp;
    *cap = new_cap;
}

/* Test if the k‑th composition (k ≥ 0) is alternating. */
static bool is_alternating(idx_t k) {
    bool first = true;          /* indicates first diff not yet processed      */
    idx_t prev_diff = 0;        /* previous part length                         */
    int prev_cmp = 0;           /* -1 or 1 for the last comparison direction   */
    idx_t prev_pos = 0;         /* previous position (starting with 0)          */
    idx_t bit = 0;
    idx_t t = k;

    while (t) {
        if (t & 1) {                      /* bit is set */
            idx_t pos = bit + 1;          /* positions start at 1                */
            idx_t diff = pos - prev_pos;  /* current part length                 */
            if (!first) {
                if (diff == prev_diff)      /* must be strictly different          */
                    return false;
                int cmp = (diff > prev_diff) ? 1 : -1;
                if (prev_cmp == cmp)       /* alternation broken                */
                    return false;
                prev_cmp = cmp;
            } else {
                first = false;            /* first part processed               */
            }
            prev_diff = diff;
            prev_pos   = pos;
        }
        t >>= 1;
        ++bit;
    }
    return true;                         /* empty or one‑part composition is vacuously alternating */
}

/* Dynamic storage of the discovered alternating indices. */
static idx_t *alt_k = NULL;
static size_t      alt_k_size = 0;
static size_t      alt_k_cap  = 0;
static idx_t      last_k       = 0;      /* next k to test           */
static idx_t      max_needed   = 0;      /* maximum n needed so far */

/* Append a new alternating index to the dynamic array. */
static void append_k(idx_t val) {
    ensure_capacity(alt_k_size + 1, &alt_k_cap, (void **)&alt_k, sizeof(idx_t));
    alt_k[alt_k_size++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* Offset is 1; out of bounds returns 0 */

    if (n > max_needed) max_needed = n;

    /* Generate enough alternating indices. */
    while (alt_k_size < max_needed) {
        if (is_alternating(last_k)) append_k(last_k);
        ++last_k;
    }
    return (seq_t)alt_k[n - 1];
}
