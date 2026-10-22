/*
 * A342297  A Van Eck-like sequence based on the Fibonacci numbers. See
 *          Comments for definition.
 * https://oeis.org/A342297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) is produced by concatenating blocks S_i. S_0 = [1]. For i ≥
 *            1, S_i has length F_i (i‑th Fibonacci number). Each element of
 *            S_i is the last position at which the value j (1 ≤ j ≤ F_i)
 *            appeared in the sequence so far. The infinite sequence a is the
 *            union of all such blocks
 * Range:     u8 n <= 415, u16 n <= 9963, u32 n <= 9963, u64 n <= 9963,
 *            u128 n <= 9963, big any n
 * Helpers:   ensure_last_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ensure_last_cap.h"

/* Grow a seq_t array to at least new_cap elements. */
static void ensure_seq_cap(idx_t min_cap, seq_t **arr, size_t *cap) {
    if (*cap >= min_cap) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 4;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *arr = tmp;
    *cap = new_cap;
}

/* Temporary buffer for a block S_i */
static seq_t *block_buf = NULL;
static size_t block_cap = 0;

seq_t a(idx_t n) {
    /* Static state retained between calls */
    static bool initialized = false;
    static seq_t *seq_arr = NULL;
    static size_t seq_cap = 0;
    static idx_t seq_len = 0;          /* number of elements generated */

    static idx_t *last_arr = NULL;     /* last position of each value */
    static size_t last_cap = 0;

    /* Fibonacci counters */
    static idx_t fib_prev = 0;          /* F_{i-1} */
    static idx_t fib_cur  = 1;          /* F_i   for next block */

    /* Initialization: S_0 */
    if (!initialized) {
        ensure_seq_cap(1, &seq_arr, &seq_cap);
        seq_arr[0] = (seq_t)1;
        seq_len = 1;

        /* last positions array: indices 0 and 1 needed */
        ensure_last_cap(2, &last_arr, &last_cap);
        last_arr[0] = 0;
        last_arr[1] = 1;               /* value 1 last seen at position 1 */

        initialized = true;
    }

    if (n == 0) return (seq_t)0;       /* outside of offset – undefined */

    /* Generate blocks until the requested index is reached */
    while ((idx_t)seq_len < n) {
        idx_t block_len = fib_cur;      /* length of next block S_i */

        /* ensure capacity for the expanding sequence */
        ensure_seq_cap(seq_len + block_len, &seq_arr, &seq_cap);

        /* ensure capacity for accessing j up to block_len */
        ensure_last_cap(block_len + 1, &last_arr, &last_cap);

        /* ensure capacity for the temporary buffer */
        if (block_cap < (size_t)block_len) {
            size_t new_cap = block_cap ? block_cap << 1 : 4;
            while (new_cap < (size_t)block_len) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(block_buf, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            block_buf = tmp;
            block_cap = new_cap;
        }

        /* First pass: compute block values from the state before this block */
        for (idx_t j = 1; j <= block_len; ++j) {
            /* last_arr[j] is an idx_t; it fits in seq_t for useful n */
            block_buf[j-1] = (seq_t)last_arr[j];
        }

        /* Second pass: append block values and update last positions */
        for (idx_t k = 0; k < block_len; ++k) {
            seq_t val = block_buf[k];
            ++seq_len;                           /* new position (1‑based) */
            seq_arr[seq_len - 1] = val;

            /* ensure last_arr can hold index val */
            if ((idx_t)val + 1 > last_cap) {
                ensure_last_cap((idx_t)val + 1, &last_arr, &last_cap);
            }
            last_arr[(idx_t)val] = seq_len;        /* update last occurrence */
        }

        /* Advance to next Fibonacci number */
        idx_t next_fib = fib_prev + fib_cur;
        fib_prev = fib_cur;
        fib_cur = next_fib;
    }

    return seq_arr[n - 1];
}
