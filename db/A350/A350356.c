/*
 * A350356  Numbers k such that the k-th composition in standard order is
 *          down/up.
 * https://oeis.org/A350356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_down_up
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search for the nth down/up composition index. */

#include <stdbool.h>
#include "../utils/is_down_up.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 0, but safe */
    idx_t target = n;
    idx_t count  = 0;
    idx_t k      = 0;

    while (1) {
        if (is_down_up(k)) {
            ++count;
            if (count == target)
                return (seq_t)k;
        }
        ++k;
    }
}
