/*
 * A304025  a(n) is the largest integer that can be written with n digits in
 *          base 3/2.
 * https://oeis.org/A304025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 2
   For n > 1:
     if a(n-1) is even: a(n) = (a(n-1)/2)*3 + 2
     if a(n-1) is odd  : a(n) = ((a(n-1)-1)/2)*3 + 2
   This recurrence follows from the integer‐representation condition
   in base 3/2.
*/

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)2;
    seq_t val = (seq_t)2;
    for (idx_t i = 2; i <= n; ++i) {
        if ((val & 1) == 0) {
            val = (val >> 1) * 3 + 2;
        } else {
            val = ((val - 1) >> 1) * 3 + 2;
        }
    }
    return val;
}
