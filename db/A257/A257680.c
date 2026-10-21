/*
 * A257680  Characteristic function for A256450: 1 if there is at least one
 *          1-digit present in the factorial representation of n (A007623),
 *          otherwise 0.
 * https://oeis.org/A257680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if the factorial-number-system representation of n
 *            contains a digit 1, otherwise 0
 * Range:     u8 n <= 8605, u16 n <= 8605, u32 n <= 8605, u64 n <= 8605,
 *            u128 n <= 8605, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t i = 1;
    while (n > 0)
    {
        idx_t d = n % (i + 1);
        if (d == 1)
            return 1;
        n = n / (i + 1);
        ++i;
    }
    return 0;
}
