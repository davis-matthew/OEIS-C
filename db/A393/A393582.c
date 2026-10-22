/*
 * A393582  Largest n-digit Zumkeller number.
 * https://oeis.org/A393582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10^n - 1 if n % 6 == 0 else 10^n - 4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t p = pow10(n);          /* p = 10^n */
    if ((n % 6) == 0)
        return p - 1;
    else
        return p - 4;
}
