/*
 * A285099  a(n) is the zero-based index of the second least significant 1-bit
 *          in the base-2 representation of n, or 0 if there are fewer than
 *          two 1-bits in n.
 * https://oeis.org/A285099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 0 if number of 1-bits in n < 2, otherwise a(n) = position
 *            (0‑based from LSB) of the second least significant 1‑bit in the
 *            binary representation of n
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, if not already defined */

seq_t a(idx_t n)
{
    /* n is unsigned 64‑bit (idx_t).  For n < 2 there are fewer than two
       1‑bits, so return 0. */
    if (n < 2U)
        return 0;

    /* Remove the least significant 1‑bit.  If the result is 0, there were
       fewer than two 1‑bits, so return 0. */
    idx_t m = n & (n - 1U);
    if (m == 0U)
        return 0;

    /* Count trailing zeros of m to find the position of that remaining
       1‑bit.  Since n fits in 64 bits, a simple loop suffices and
       works in C11 and C++17. */
    idx_t pos = 0;
    while ((m & 1U) == 0U) {
        pos++;
        m >>= 1U;
    }

    return (seq_t)pos;
}
