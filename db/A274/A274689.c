/*
 * A274689  A variation of A005228.
 * https://oeis.org/A274689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 55, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  Iteratively construct A274689.
   Starting with a(1)=1, at each step choose a diff k >= 1 that
   satisfies:
     - a(n+1)=a(n)+k (or a(n)-k) ≠ 0
     - neither a(n+1) nor k (or –k) has appeared before in
       the sequence or as a first difference.
   Among all admissible k the smallest |k| is chosen.
   If both signs are admissible, the term giving the smaller
   |a(n+1)| is preferred; ties are broken by the smaller
   numeric value (lexicographically earliest). 
   All used integers are stored in a single array to allow
   fast membership tests.                                       */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper: absolute value for seq_t */
static seq_t abs_seq_t(seq_t x)
{
    return x < (seq_t)0 ? -x : x;
}

/* Helper: test whether 'x' is in a scalar array 'arr' of length 'len' */
static bool contains(const seq_t *arr, size_t len, seq_t x)
{
    for (size_t i = 0; i < len; ++i)
        if (arr[i] == x)
            return true;
    return false;
}

/* Helper: grow a dynamic array if needed */
static void ensure_capacity(void **ptr, size_t *len, size_t *cap, size_t elem_size, size_t extra)
{
    size_t needed = *len + extra;
    if (needed > *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        while (new_cap < needed)
            new_cap <<= 1;
        void *p = realloc(*ptr, new_cap * elem_size);
        if (!p)
            exit(EXIT_FAILURE);
        *ptr = p;
        *cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static tables for computed terms and all used integers */
    static seq_t *seq     = NULL; /* sequence terms   */
    static size_t seq_len = 0;
    static size_t seq_cap = 0;

    static seq_t *used    = NULL; /* terms + differences */
    static size_t used_len = 0;
    static size_t used_cap = 0;

    if (n == 0)       /* offset is 1; undefined below, return 0 */
        return (seq_t)0;

    /* initialise on first call */
    if (seq_len == 0) {
        ensure_capacity((void**)&seq, &seq_len, &seq_cap, sizeof(seq_t), 1);
        ensure_capacity((void**)&used, &used_len, &used_cap, sizeof(seq_t), 2);
        seq[0]     = (seq_t)1;
        seq_len    = 1;
        used[0]    = (seq_t)1;     /* term 1 used          */
        used[1]    = (seq_t)0;     /* placeholder for diff */
        used_len   = 1;            /* only term used yet   */
    }

    /* compute up to required index */
    while (seq_len < (size_t)n) {
        seq_t prev = seq[seq_len - 1];
        seq_t chosen_val, chosen_diff;
        bool found = false;

        /* search increasing |k| */
        for (idx_t k = 1; !found; ++k) {
            seq_t diff_pos = (seq_t)k;
            seq_t next_pos = prev + diff_pos;
            bool pos_ok = true;
            if (next_pos == (seq_t)0 || contains(used, used_len, next_pos) || contains(used, used_len, diff_pos))
                pos_ok = false;

            seq_t diff_neg = -diff_pos;
            seq_t next_neg = prev + diff_neg;
            bool neg_ok = true;
            if (next_neg == (seq_t)0 || contains(used, used_len, next_neg) || contains(used, used_len, diff_neg))
                neg_ok = false;

            if (pos_ok || neg_ok) {
                if (pos_ok && neg_ok) {
                    seq_t abs_pos = abs_seq_t(next_pos);
                    seq_t abs_neg = abs_seq_t(next_neg);
                    if (abs_pos < abs_neg) {
                        chosen_val  = next_pos;
                        chosen_diff = diff_pos;
                    } else if (abs_neg < abs_pos) {
                        chosen_val  = next_neg;
                        chosen_diff = diff_neg;
                    } else {
                        /* tie on |a(n+1)| : choose lexicographically earliest (smaller value) */
                        if (next_pos < next_neg) {
                            chosen_val  = next_pos;
                            chosen_diff = diff_pos;
                        } else {
                            chosen_val  = next_neg;
                            chosen_diff = diff_neg;
                        }
                    }
                } else if (pos_ok) {
                    chosen_val  = next_pos;
                    chosen_diff = diff_pos;
                } else { /* neg_ok only */
                    chosen_val  = next_neg;
                    chosen_diff = diff_neg;
                }
                found = true;
            }
        }

        /* append new term and diff */
        ensure_capacity((void**)&seq, &seq_len, &seq_cap, sizeof(seq_t), 1);
        seq[seq_len++] = chosen_val;

        /* store used integer pair (value, diff) */
        ensure_capacity((void**)&used, &used_len, &used_cap, sizeof(seq_t), 2);
        used[used_len++] = chosen_val;
        used[used_len++] = chosen_diff;
    }

    return seq[n - 1];
}
