/*
 * A253621  Indices of centered heptagonal numbers (A069099) which are also
 *          centered pentagonal numbers (A005891).
 * https://oeis.org/A253621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 13*a(n-2) + a(n-3) with a(1)=1, a(2)=6,
 *            a(3)=66
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for constexpr constants, may be ignored in the build */

seq_t a(idx_t n)
{
    /* Base terms */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)6;
    if (n == 3)
        return (seq_t)66;

    /* Iterative computation for n >= 4 */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)6;   /* a(n-2) */
    seq_t a3 = (seq_t)66;  /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 13 * a3 - 13 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
