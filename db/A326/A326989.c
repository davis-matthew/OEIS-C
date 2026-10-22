/*
 * A326989  Triangle read by rows in which row n lists the nonpowers of 2
 *          dividing n, or 0 if n is a power of 2.
 * https://oeis.org/A326989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = the k‑th non‑power‑of‑2 divisor of m, where m is the
 *            smallest integer with Σ_{i=1}^{m} A326987(i) ≥ n. A326987(i) = 1
 *            if i is a power of 2, otherwise the number of divisors of i that
 *            are not powers of 2
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    idx_t row = 1;
    idx_t sum = 0;

    while (1) {
        /* length of current row */
        seq_t row_s = (seq_t)row;
        idx_t len = 0;

        if (is_power_of_two(row_s)) {
            len = 1;
        } else {
            for (idx_t d = 1; d * d <= row; ++d) {
                if (row % d == 0) {
                    seq_t d_s = (seq_t)d;
                    if (!is_power_of_two(d_s))
                        ++len;
                    idx_t d2 = row / d;
                    if (d2 != d) {
                        seq_t d2_s = (seq_t)d2;
                        if (!is_power_of_two(d2_s))
                            ++len;
                    }
                }
            }
        }

        if (sum + len >= n) {          /* row found */
            idx_t k = n - sum - 1;     /* zero‑based index in the row */

            if (is_power_of_two(row_s))
                return 0;              /* row of a power of 2 */

            /* collect non‑power‑of‑2 divisors of row */
            seq_t divs[64];
            idx_t cnt = 0;
            for (idx_t d = 1; d * d <= row; ++d) {
                if (row % d == 0) {
                    seq_t d_s = (seq_t)d;
                    if (!is_power_of_two(d_s))
                        divs[cnt++] = d_s;
                    idx_t d2 = row / d;
                    if (d2 != d) {
                        seq_t d2_s = (seq_t)d2;
                        if (!is_power_of_two(d2_s))
                            divs[cnt++] = d2_s;
                    }
                }
            }

            /* sort ascending (insertion sort, small array) */
            for (idx_t i = 1; i < cnt; ++i) {
                seq_t key = divs[i];
                idx_t j = i;
                while (j > 0 && divs[j - 1] > key) {
                    divs[j] = divs[j - 1];
                    --j;
                }
                divs[j] = key;
            }

            return divs[k];
        }

        sum += len;
        ++row;
    }
}
