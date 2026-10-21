/*
 * A295841  Number of n X 2 0..1 arrays with each 1 adjacent to 1 or 2
 *          king-move neighboring 1s.
 * https://oeis.org/A295841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + 6*a(n-3) - 8*a(n-4) Initial values:
 *            a(1)=2, a(2)=11, a(3)=29, a(4)=80
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; a(0) is unused. */
    if (n == 0) return 0;

    if (n == 1) return 2;
    if (n == 2) return 11;
    if (n == 3) return 29;
    if (n == 4) return 80;

    seq_t v1 = 2, v2 = 11, v3 = 29, v4 = 80;   /* a(n-4) .. a(n-1) */
    seq_t next;
    for (idx_t i = 5; i <= n; ++i) {
        next = 3 * v4 - v3 + 6 * v2 - 8 * v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = next;
    }
    return v4;
}
