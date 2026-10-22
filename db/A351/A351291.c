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
 * Helpers:   has_duplicate_runs
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
#include "../utils/has_duplicate_runs.h"

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
