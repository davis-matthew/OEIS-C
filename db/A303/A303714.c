/*
 * A303714  Number of n X 3 0..1 arrays with every element unequal to 0, 1 or
 *          5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) for n > 4; initial values: a(1)=3,
 *            a(2)=3, a(3)=5, a(4)=8
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 42, u64 n <= 84, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* For n below the offset behavior does not matter; return 0 */
    if (n == 0) return 0;

    /* Handle the four known initial values directly */
    if (n == 1) return 3;
    if (n == 2) return 3;
    if (n == 3) return 5;
    if (n == 4) return 8;

    /* Sliding‑window variables:
       v1 = a(n-4), v2 = a(n-3), v3 = a(n-2), v4 = a(n-1)   */
    seq_t v1 = (seq_t)3;              /* a(1) */
    seq_t v2 = (seq_t)3;              /* a(2) */
    seq_t v3 = (seq_t)5;              /* a(3) */
    seq_t v4 = (seq_t)8;              /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = v4 + (v2 * (seq_t)2);  /* a(i-1) + 2*a(i-3) */
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = next;
    }
    return v4;
}
