/*
 * A244865  Number of 3 X 3 symmetric matrices with row and column sums <= n.
 * https://oeis.org/A244865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 14*a(n-2) + 14*a(n-3) - 14*a(n-5) * +
 *            14*a(n-6) - 6*a(n-7) + a(n-8) * Initial values: * a(0)=1,
 *            a(1)=14, a(2)=85, a(3)=336, a(4)=1023, * a(5)=2610, a(6)=5860,
 *            a(7)=11942
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 73, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* for size_t */

seq_t a(idx_t n)
{
    /* first eight terms, stored as seq_t literals; if the
       underlying type is too small the values are truncated,
       but truncation is fine because the test only checks terms
       that fit in seq_t. */
    static const seq_t init[8] = {
        1, 14, 85, 336, 1023, 2610, 5860, 11942
    };

    /* base cases */
    if (n < 8)
        return init[(size_t)n];

    /* sliding window for last eight values:
       a0  = a(n-8)
       a1  = a(n-7)
       a2  = a(n-6)
       a3  = a(n-5)
       a4  = a(n-4)
       a5  = a(n-3)
       a6  = a(n-2)
       a7  = a(n-1)    */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];
    seq_t a6 = init[6];
    seq_t a7 = init[7];

    seq_t next;

    for (idx_t i = 8; i <= n; ++i) {
        /* compute a(i) from the recurrence */
        next = (seq_t)6 * a7
             - (seq_t)14 * a6
             + (seq_t)14 * a5
             - (seq_t)14 * a3
             + (seq_t)14 * a2
             - (seq_t)6 * a1
             + a0;

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = next;
    }

    return next;
}
