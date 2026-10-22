/*
 * A333344  a(n) = 11*a(n-1) - 9*a(n-2) starting a(0)=1, a(1)=10.
 * https://oeis.org/A333344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(1) = 10; a(n) = 11*a(n-1) - 9*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)10;  /* a(1) */
    if (n == 1) {
        return a1;
    }
    seq_t an = 0;
    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * (seq_t)11 - a0 * (seq_t)9;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
