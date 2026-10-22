/*
 * A344619  The a(n)-th composition in standard order (A066099) has
 *          alternating sum 0.
 * https://oeis.org/A344619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th nonnegative integer k for which the composition
 *            obtained from k in the standard order of A066099 has alternating
 *            sum 0. Thus a(1)=0, a(2)=3, a(3)=10,
 * Range:     u8 n <= 50, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   alt_sum_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alt_sum_zero.h"

seq_t a(idx_t n)
{
    /* Find the n‑th index k (starting from 0) with alternating sum 0. */
    idx_t count = 0;
    for (idx_t k = 0;; ++k) {
        if (alt_sum_zero(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
