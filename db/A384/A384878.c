/*
 * A384878  Position of first appearance of n in the flattened version of the
 *          triangle A384877, whose m-th row lists the lengths of maximal
 *          anti-runs in the binary indices of m.
 * https://oeis.org/A384878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Computes a(n), the position of the first appearance of n
   in the flattened triangle of maximal anti‑run lengths of the binary
   indices of positive integers.  The anti‑runs are obtained by a greedy
   partition: starting with the smallest index, include subsequent indices
   that are not consecutive with any already selected in the current run.
*/
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* no term for 0 */

    seq_t pos = 0;                     /* current global position */

    for (idx_t m = 1; ; ++m) {
        bool in_run = false;
        idx_t run_len = 0;
        idx_t last_index = 0;

        idx_t temp = m;
        idx_t bit_pos = 1;
        while (temp) {
            if (temp & 1) {            /* bit at position bit_pos is 1 */
                if (!in_run) {
                    in_run = true;
                    run_len = 1;
                    last_index = bit_pos;
                } else if (bit_pos == last_index + 1) {
                    /* finish current run */
                    ++pos;
                    if ((seq_t)run_len == (seq_t)n)
                        return pos;
                    /* start new run with this index */
                    run_len = 1;
                    last_index = bit_pos;
                } else {
                    /* extend current run */
                    ++run_len;
                    last_index = bit_pos;
                }
            }
            temp >>= 1;
            ++bit_pos;
        }

        if (in_run) {                  /* finish the last run of this row */
            ++pos;
            if ((seq_t)run_len == (seq_t)n)
                return pos;
        }
    }
}
