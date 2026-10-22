/*
 * A350355  Numbers k such that the k-th composition in standard order is
 *          up/down.
 * https://oeis.org/A350355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_up_down
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the non‑negative integer k that is the nth composition,
 *             in standard order, which is up/down (alternating increasing
 *             and decreasing, starting with an increase).  The sequence
 *             values are the indices k, with a(1)=0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_up_down.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t found = 0;          /* how many up/down compositions found */
    uint64_t k = 0;           /* current index (non‑negative integer) */

    while (1) {
        if (is_up_down(k)) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        ++k;
    }
}
