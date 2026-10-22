/*
 * A356678  The lexicographically earliest infinite squarefree sequence of
 *          nonnegative integers that starts with 2.
 * https://oeis.org/A356678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the lexicographically earliest infinite squarefree sequence
   of nonnegative integers starting with 2.  The sequence is built
   incrementally:  a(1)=2, and for n>1 we choose the smallest integer
   that, when appended, creates no adjacent equal block of any length. */

/* Include standard headers */
#include <stdbool.h>
#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for abort */

/* Utilities for dynamic array management */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* ----------  static state  ---------- */
static seq_t *seq_arr = NULL;   /* array of computed terms          */
static size_t seq_len = 0;      /* number of terms currently stored */
static size_t seq_cap = 0;      /* capacity of seq_arr              */

/* ----------  helper to grow the array  ---------- */
static void grow_seq(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = alloc_seq_array(new_cap);
    if (!new_arr) abort();          /* out of memory */
    for (size_t i = 0; i < seq_len; ++i) new_arr[i] = seq_arr[i];
    if (seq_arr) free_seq_array(seq_arr);
    seq_arr = new_arr;
    seq_cap = new_cap;
}

/* ----------  generate terms up to at least n  ---------- */
static void fill_to(idx_t n)
{
    size_t target = (size_t)n;          /* number of terms needed   */
    if (target == 0) return;
    /* Ensure capacity and initialise first term if needed */
    if (seq_len == 0) {
        grow_seq(1);
        seq_arr[0] = 2;                 /* a(1) = 2 */
        seq_len = 1;
    }
    if (target <= seq_len) return;
    grow_seq(target);

    /* Append terms until we have target terms */
    for (size_t pos = seq_len; pos < target; ++pos) {
        seq_t cand = 0;                 /* candidate to test     */
        for (;;) {                      /* find smallest legal   */
            bool bad = false;           /* candidate creates a square? */

            /* Only lengths l for which 2*l <= pos+1 can end at pos */
            for (idx_t l = 1; (l << 1) <= ((idx_t)pos + 1); ++l) {
                idx_t start = (idx_t)pos - (l << 1) + 1; /* start of potential square */
                bool all_equal = true;
                for (idx_t k = 0; k < l; ++k) {
                    seq_t left  = seq_arr[(size_t)start + (size_t)k];
                    idx_t   right_idx = start + l + k;
                    seq_t right = (right_idx == (idx_t)pos) ? cand : seq_arr[(size_t)right_idx];
                    if (left != right) { all_equal = false; break; }
                }
                if (all_equal) { bad = true; break; } /* square found */
            }
            if (!bad) break;           /* candidate accepted      */
            ++cand;                     /* try next value          */
        }
        seq_arr[pos] = cand;
        seq_len++;
    }
}

/* ----------  public function  ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, safe default */
    fill_to(n);
    return seq_arr[(size_t)n - 1];
}
