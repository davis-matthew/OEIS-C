/*
 * A344109  a(n) = (5*2^n + 7*(-1)^n)/3.
 * https://oeis.org/A344109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) with a(0) = 4, a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 4;
    if (n == 1) return 1;

    seq_t a0 = 4; /* a(i-2)   */
    seq_t a1 = 1; /* a(i-1)   */
    seq_t an = 0; /* current a(i) */

    for (idx_t i = 2; i <= n; ++i) {
        an = a1 + (seq_t)2 * a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}
