/*
 * A322709  a(0)=0, a(1)=7 and a(n) = 30*a(n-1) - a(n-2) + 14 for n > 1.
 * https://oeis.org/A322709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*a(n-1) - a(n-2) + 14, a(0)=0, a(1)=7
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)7;
    seq_t a0 = (seq_t)0;      /* a(0) */
    seq_t a1 = (seq_t)7;      /* a(1) */
    seq_t an = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i) {
        an = ((seq_t)30 * a1) - a0 + (seq_t)14;
        a0 = a1;
        a1 = an;
    }
    return an;
}
