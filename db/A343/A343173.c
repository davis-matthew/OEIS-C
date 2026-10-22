/*
 * A343173  First differences of paper-folding sequence A014577.
 * https://oeis.org/A343173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = s(n+1) - s(n), where s is the regular paper‑folding
 *            sequence A014577 (binary 0/1). This implementation builds the
 *            binary sequence up to index n+1 and returns the required
 *            difference. It uses the provided alloc_seq_array/free_seq_array
 *            helpers to safely handle arbitrary‑precision seq_t types
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    /* Find the length L of the paper‑folding sequence that contains
       indices 0 … n+1.  After k iterations the length is 2^k‑1, so
       we double until we exceed n+1. */
    size_t cur_len = 1;
    while (cur_len <= (size_t)n + 1) {
        /* careful: cur_len*2+1 will not overflow for n up to at least
           10 000 as required by the time limit. */
        cur_len = cur_len * 2 + 1;
    }

    /* Allocate an array of seq_t with proper construction (needed when
       seq_t is an arbitrary‑precision type). */
    seq_t *seq = alloc_seq_array(cur_len);
    if (!seq) {
        /* allocation failed – return 0 (sequence entry 0) as safe fallback */
        return (seq_t)0;
    }

    /* Build the paper‑folding sequence.  Start with the single element
       1, then repeatedly extend: old + 1 + complement(reverse(old)). */
    seq[0] = (seq_t)1;
    size_t len = 1;                     /* current usable length */
    while (len <= (size_t)n + 1) {
        size_t new_len = len * 2 + 1;
        seq[len] = (seq_t)1;            /* middle element */

        /* complement(reverse(old)) */
        for (size_t i = 0; i < len; ++i) {
            /* index of old element to reverse */
            size_t rev_index = len - 1 - i;
            /* 1 - old = complement (since old is 0 or 1) */
            seq[len + 1 + i] = (seq_t)((seq_t)1 - seq[rev_index]);
        }
        len = new_len;
    }

    /* Compute the first difference a(n) = s(n+1) - s(n). */
    seq_t diff = seq[(size_t)n + 1] - seq[(size_t)n];

    /* Clean up. */
    free_seq_array(seq);
    return diff;
}
