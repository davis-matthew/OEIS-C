/*
 * A333489  Numbers k such that the k-th composition in standard order is an
 *          anti-run (no adjacent equal parts).
 * https://oeis.org/A333489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th index k such that the k-th composition in
 *            standard order is an anti‐run (no adjacent equal parts)
 * Range:     u8 n <= 93, u16 n <= 8259, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_anti_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_anti_run.h"

seq_t a(idx_t n)
{
    /* a(1) = 0, a(2) = 1, ...   (offset 1) */
    idx_t count = 0;
    idx_t k     = 0;

    while (true)
    {
        if (is_anti_run(k))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
