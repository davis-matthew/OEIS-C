/*
 * A276849  a(0) = 5, a(1) = 2; for n>1, a(n) = 2*a(n-1) + a(n-2).
 * https://oeis.org/A276849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 5, a(1) = 2; a(n) = 2*a(n-1) + a(n-2) for n > 1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)5;
    }
    if (n == 1) {
        return (seq_t)2;
    }

    seq_t a_prev2 = (seq_t)5;  /* a(0) */
    seq_t a_prev1 = (seq_t)2;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 2 * a_prev1 + a_prev2;  /* a(i) = 2*a(i-1) + a(i-2) */
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }

    return a_prev1;
}
