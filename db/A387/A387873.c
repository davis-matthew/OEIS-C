/*
 * A387873  a(n) = Sum_{k=0..n} binomial(4*n+1,5*k).
 * https://oeis.org/A387873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=127, a(3)=1574, a(4)=25773,
 *            a(n)=5*a(n-1)+130*a(n-2)+740*a(n-3)-65*a(n-4)+16*a(n-5) for n>4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0..4 */
    static const seq_t init[5] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)127,
        (seq_t)1574,
        (seq_t)25773
    };

    if (n < 5)
        return init[n];

    /* Variables hold a(n-5) .. a(n-1) */
    seq_t a_nm5 = init[0]; /* a(n-5) */
    seq_t a_nm4 = init[1]; /* a(n-4) */
    seq_t a_nm3 = init[2]; /* a(n-3) */
    seq_t a_nm2 = init[3]; /* a(n-2) */
    seq_t a_nm1 = init[4]; /* a(n-1) */
    seq_t a_cur = 0;

    for (idx_t i = 5; i <= n; ++i) {
        /* a_cur = 5*a_nm1 + 130*a_nm2 + 740*a_nm3 + 16*a_nm5 - 65*a_nm4;
           Compute as two steps to avoid overflow in intermediate sums. */
        seq_t tmp = 5 * a_nm1 + 130 * a_nm2 + 740 * a_nm3 + 16 * a_nm5;
        a_cur = tmp - 65 * a_nm4;

        /* Shift the window */
        a_nm5 = a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }

    return a_cur;
}
