/*
 * A253476  Indices of centered triangular numbers (A005448) which are also
 *          centered heptagonal numbers (A069099).
 * https://oeis.org/A253476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=a(n-1)+110*a(n-2)-110*a(n-3)-a(n-4)+a(n-5) with a(1)=1,
 *            a(2)=15, a(3)=70, a(4)=1596, a(5)=7645
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* handle the offset: the sequence is defined for n >= 1 */
    if (n == 0)
        return 0;    /* undefined for n < 1, return 0 as a safe default */

    /* initial values for n = 1 .. 5 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)70;
    if (n == 4) return (seq_t)1596;
    if (n == 5) return (seq_t)7645;

    /* use a small sliding window of size 5 to iterate the recurrence */
    seq_t t[5];
    t[0] = (seq_t)1;
    t[1] = (seq_t)15;
    t[2] = (seq_t)70;
    t[3] = (seq_t)1596;
    t[4] = (seq_t)7645;

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = t[4]
                     + (seq_t)110 * t[3]
                     - (seq_t)110 * t[2]
                     - t[1]
                     + t[0];
        /* slide window: drop oldest and append newest */
        t[0] = t[1];
        t[1] = t[2];
        t[2] = t[3];
        t[3] = t[4];
        t[4] = next;
    }
    return t[4];
}
