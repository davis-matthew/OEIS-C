/*
 * A356681  The lexicographically earliest infinite squarefree sequence of
 *          nonnegative integers that starts with 1, 3.
 * https://oeis.org/A356681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: lexicographically earliest square‑free sequence over non‑negative
 * integers starting with 1, 3.  Computed incrementally by testing each
 * candidate for the emergence of a square (subsequence XX). */

#include <stdlib.h>
#include <stdbool.h>

/* static state: the sequence array, its length and capacity */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* grow the array to at least 'needed' elements */
static void ensure_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1;
    if (new_cap < needed) new_cap = needed;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* test whether appending 'cand' to the existing sequence would create a
 * square of length >=1.  The sequence before appending has length 'seq_len'.
 * Returns true if a square would appear, false otherwise. */
static bool would_create_square(seq_t cand)
{
    /* maximum half‑length that could end at the new position */
    size_t max_l = (seq_len + 1) / 2;
    for (size_t l = 1; l <= max_l; ++l) {
        size_t left_start  = seq_len - 2 * l + 1;
        size_t right_start = seq_len - l + 1;
        size_t i;
        for (i = 0; i < l; ++i) {
            seq_t left  = seq[left_start + i];
            seq_t right = (right_start + i == seq_len) ? cand : seq[right_start + i];
            if (left != right) break;
        }
        if (i == l) return true;   /* a square of length l has been found */
    }
    return false;
}

/* Main access function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n < 1, but harmless */

    /* initialise with the given first two terms */
    if (seq_len == 0) {
        ensure_cap(2);
        seq[0] = 1;
        seq[1] = 3;
        seq_len = 2;
    }

    /* extend the sequence until it contains at least n terms */
    while (seq_len < (size_t)n) {
        seq_t cand = 0;                 /* start checking from 0 upwards */
        while (true) {
            if (!would_create_square(cand)) break;   /* candidate accepted */
            ++cand;
        }
        ensure_cap(seq_len + 1);
        seq[seq_len++] = cand;
    }

    return seq[n - 1];
}
