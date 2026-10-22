/*
 * A374633  Numbers k such that the leaders of weakly increasing runs in the
 *          k-th composition in standard order (A066099) are identical.
 * https://oeis.org/A374633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer k such that the leaders of weakly increasing
 *            runs in the k-th composition in standard order (OEIS A066099)
 *            are all equal. Computation is done by a linear search over k (no
 *            closed form)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   is_good_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_good_k.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0; ; ++k) {
        if (is_good_k(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
