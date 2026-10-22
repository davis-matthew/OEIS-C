/*
 * A399042  Total number of connected components in all induced subgraphs of
 *          the 4 X n grid graph.
 * https://oeis.org/A399042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*a(n-1) - 386*a(n-2) + 1087*a(n-3) - 480*a(n-4) -
 *            256*a(n-5) with a(1)=20, a(2)=432, a(3)=8721, a(4)=168529,
 *            a(5)=3160507
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1, but safe */

    /* initial values */
    static const seq_t init[5] = {
        (seq_t)20,
        (seq_t)432,
        (seq_t)8721,
        (seq_t)168529,
        (seq_t)3160507
    };

    if (n <= 5) return init[n-1];

    /* compute iteratively; only last 5 terms are needed */
    seq_t a1 = init[0];
    seq_t a2 = init[1];
    seq_t a3 = init[2];
    seq_t a4 = init[3];
    seq_t a5 = init[4];
    seq_t next;

    for (idx_t i = 6; i <= n; ++i) {
        next = (seq_t)36 * a5
             - (seq_t)386 * a4
             + (seq_t)1087 * a3
             - (seq_t)480 * a2
             - (seq_t)256 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
