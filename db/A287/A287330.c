/*
 * A287330  7*x - 1 Collatz-type sequence starting with a(0) = 11.
 * https://oeis.org/A287330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 11; a(n) = 7*a(n-1)-1 if a(n-1) is odd,
   a(n) = a(n-1)/2 if a(n-1) is even. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)11;
    seq_t val = (seq_t)11;
    for (idx_t i = 1; i <= n; ++i) {
        if ((val & (seq_t)1) != 0)  /* odd */
            val = val * (seq_t)7 - (seq_t)1;
        else                       /* even */
            val = val / (seq_t)2;
    }
    return val;
}
