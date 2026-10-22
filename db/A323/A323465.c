/*
 * A323465  Irregular triangle read by rows: row n lists the numbers that can
 *          be obtained from the binary expansion of n by either deleting a
 *          single 0, or inserting a single 0 after any 1, or doing nothing.
 * https://oeis.org/A323465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look, base
 * Formula:   not stated
 * Range:     u8 n <= 768, u16 n <= 8208, u32 n <= 8208, u64 n <= 8208,
 *            u128 n <= 8208, big any n
 * Helpers:   bin_to_num, generate_row, idx_to_bin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323465
   The nth term is the nth element of the irregular triangle
   flattened row-wise.  Row n contains all numbers obtainable
   from the binary expansion of n by either:
      • deleting a single 0,
      • inserting a single 0 after any 1,
      • doing nothing.
   Each row is sorted ascending and then concatenated. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/bin_to_num.h"
#include "../utils/generate_row.h"
#include "../utils/idx_to_bin.h"

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t target = n;
    idx_t cumulative = 0;

    for (idx_t row = 1; ; ++row) {
        seq_t row_vals[200];
        size_t row_cnt;
        generate_row(row, row_vals, &row_cnt);

        if (target <= cumulative + (idx_t)row_cnt) {
            return row_vals[target - cumulative - 1];
        }
        cumulative += row_cnt;
    }
}
