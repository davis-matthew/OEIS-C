/*
 * A353442  Integers m such that the decimal expansion of 1/m contains the
 *          digit 6.
 * https://oeis.org/A353442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   contains6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sequential search for integers m such that the decimal expansion of 1/m contains the digit 6. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/contains6.h"

/* Main sequence function: a(n) returns the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined for n < 1 */
        return 0;

    idx_t count = 0;
    seq_t m = 1;
    while (true)
    {
        ++m;                          /* next integer to test */
        if (contains6(m))
        {
            ++count;
            if (count == n)
                return m;
        }
    }
}
