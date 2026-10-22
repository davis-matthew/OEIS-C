/*
 * A335718  a(n) = 2*a(n-1) + 3*a(n-2) + 5*a(n-3), a(0) = 0, a(1) = 1, a(2) =
 *          2.
 * https://oeis.org/A335718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) + 5*a(n-3), a(0) = 0, a(1) = 1, a(2)
 *            = 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Iteratively compute terms up to n.  Using three variables to save
       memory and avoid array overhead. */
    seq_t a0 = (seq_t)0; /* a(i-3) */
    seq_t a1 = (seq_t)1; /* a(i-2) */
    seq_t a2 = (seq_t)2; /* a(i-1) */
    seq_t an = 0;

    for (idx_t i = 3; i <= n; ++i) {
        an = 2 * a2 + 3 * a1 + 5 * a0; /* a(i) = 2*a(i-1)+3*a(i-2)+5*a(i-3) */
        a0 = a1; /* shift window */
        a1 = a2;
        a2 = an;
    }

    return an;
}
