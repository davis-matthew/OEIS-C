/*
 * A340733  a(0) = 0; for n > 0, if the value of n itself appears in the
 *          sequence then a(n) = a(n-1) - (n-index(n)) if nonnegative and not
 *          already in the sequence, otherwise a(n) = a(n-1) + (n-index(n)),
 *          where index(n) is the index of the last appearance of n. If the
 *          value of n does not appear then a(n) = a(...
 * https://oeis.org/A340733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; For n > 0 let step = n if n has not appeared before,
 *            step = n − index(n) otherwise, where index(n) is the last index
 *            where the value n occurs. Let prev = a(n−1). If prev ≥ step and
 *            prev−step has not yet appeared, then a(n) = prev−step; otherwise
 *            a(n) = prev+step
 * Range:     u8 n <= 67, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* static storage for the sequence (growable) */
static seq_t  *seq       = NULL;
static idx_t   seq_len   = 0;   /* next free index (size of stored part)   */
static idx_t   seq_cap   = 0;   /* allocated capacity */

/* reverse linear search for a value; on success out_last receives the
   last index where val occurs.  Returns true if found.                    */
static bool value_exists(seq_t val, idx_t upto, idx_t *out_last)
{
    for (idx_t i = upto; i > 0; --i) {
        if (seq[i-1] == val) {
            if (out_last) *out_last = i-1;
            return true;
        }
    }
    return false;
}

/* grow the array to at least 'needed' capacity, copying old entries.   */
static void ensure_capacity(idx_t needed)
{
    if (seq_cap >= needed) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;

    seq_t *new_arr = alloc_seq_array(new_cap);
    for (idx_t i = 0; i < seq_len; ++i) new_arr[i] = seq[i];
    free_seq_array(seq);
    seq        = new_arr;
    seq_cap    = new_cap;
}

/* Main entry point: compute a(n) for arbitrary order of calls.          */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* initialise first term if not already done */
        if (!seq) {
            ensure_capacity(1);
            seq[0] = 0;
            seq_len = 1;
        }
        return 0;
    }

    /* Generate the sequence up to n if necessary */
    if (seq_len <= n) {
        ensure_capacity(n + 1);
        for (idx_t k = seq_len; k <= n; ++k) {
            seq_t step;
            idx_t last_pos = 0;
            if (value_exists((seq_t)k, seq_len, &last_pos)) {
                /* n has appeared before; step = n - last_index */
                step = (seq_t)k - (seq_t)last_pos;
            } else {
                step = (seq_t)k;
            }

            seq_t prev = seq[k-1];
            if (prev >= step) {
                seq_t cand = prev - step;
                if (!value_exists(cand, k, NULL)) {
                    seq[k] = cand;
                    continue;
                }
            }
            seq[k] = prev + step;
        }
        seq_len = n + 1;
    }
    return seq[n];
}
