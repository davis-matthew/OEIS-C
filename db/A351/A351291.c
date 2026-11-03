/*
 * A351291  Numbers k such that the k-th composition in standard order does
 *          not have all distinct runs.
 * https://oeis.org/A351291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A351291 – Numbers k such that the k‑th composition in standard order
 *  (graded reverse–lexicographic, A066099) does not have all distinct runs.
 *
 *  The function scans k from 1 upward, computes its standard‑order composition
 *  from the binary representation of k, and checks whether any run
 *  (maximal block of equal consecutive parts) repeats.
 *  The nth such k is returned as a(seq_t).
 */

#include <stdbool.h>
#include <stddef.h>   /* size_t */

static bool has_duplicate_runs(idx_t k)
{
    /* Positions of 1 bits in the binary expansion of k with LSB at position 1.
       The number of these positions equals popcount(k). */
    idx_t positions[64];
    size_t pos_cnt = 0;
    idx_t bit_index = 1;
    idx_t tmp = k;
    while (tmp) {
        if (tmp & (idx_t)1) {
            positions[pos_cnt++] = bit_index;
        }
        tmp >>= 1;
        ++bit_index;
    }
    /* For k >= 1, pos_cnt >= 1.  */
    if (pos_cnt == 0) return false;   /* safety for k==0 */

    /* Compute the composition parts: prepend 0, take first differences. */
    idx_t parts[64];
    size_t part_len = pos_cnt;          /* number of parts = popcount(k) */
    parts[0] = positions[0];
    for (size_t i = 1; i < part_len; ++i)
        parts[i] = positions[i] - positions[i - 1];

    /* Run detection on the reversed composition (obtained by reversing parts). */
    idx_t run_val[64];
    idx_t run_len[64];
    size_t run_cnt = 0;

    /* Initialise with the first element of the reversed composition. */
    idx_t cur_val = parts[part_len - 1];
    idx_t cur_len = 1;

    /* Iterate over the rest of the reversed composition. */
    for (size_t rev_i = part_len - 2; rev_i != (size_t)(-1); --rev_i) {
        idx_t part = parts[rev_i];
        if (part == cur_val) {
            ++cur_len;
        } else {
            /* Check whether the run has appeared before. */
            for (size_t j = 0; j < run_cnt; ++j) {
                if (run_val[j] == cur_val && run_len[j] == cur_len)
                    return true;
            }
            /* Store this run. */
            run_val[run_cnt] = cur_val;
            run_len[run_cnt] = cur_len;
            ++run_cnt;

            cur_val = part;
            cur_len = 1;
        }
    }

    /* Check the final run. */
    for (size_t j = 0; j < run_cnt; ++j) {
        if (run_val[j] == cur_val && run_len[j] == cur_len)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        if (has_duplicate_runs(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
