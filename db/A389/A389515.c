/*
 * A389515  Standard composition numbers of compositions whose maximal runs R
 *          all satisfy length(R) > first(R).
 * https://oeis.org/A389515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th non‑negative integer k such that the composition
 *            obtained from k in standard order has every maximal run R
 *            satisfy length(R) > first(R)
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   comp_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/comp_ok.h"

seq_t a(idx_t n)
{
    idx_t k = 0;          /* current index in standard order          */
    idx_t count = 0;      /* how many qualifying compositions seen   */

    while (true) {
        if (comp_ok(k)) {
            count++;
            if (count == n)
                return (seq_t)k;
        }
        k++;
    }
}
