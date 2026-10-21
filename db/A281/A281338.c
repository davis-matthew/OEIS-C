/*
 * A281338  Number of n X 2 0..1 arrays with no element equal to more than
 *          three of its king-move neighbors and with new values introduced in
 *          order 0 sequentially upwards.
 * https://oeis.org/A281338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + 2*a(n-3) - 2*a(n-4) - 4*a(n-5) for n
 *            ≥ 6; a(1)=2, a(2)=8, a(3)=25, a(4)=85, a(5)=286
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* offset is 1; undefined for n=0; return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* handle the first five terms directly */
    if (n <= 5) {
        static const seq_t init[5] = {
            (seq_t)2,   /* a(1) */
            (seq_t)8,   /* a(2) */
            (seq_t)25,  /* a(3) */
            (seq_t)85,  /* a(4) */
            (seq_t)286  /* a(5) */
        };
        return init[n-1];
    }

    /* iterate from n = 6 up to the requested n, keeping only last five values */
    seq_t a_nm5 = (seq_t)2;    /* a(n-5) */
    seq_t a_nm4 = (seq_t)8;    /* a(n-4) */
    seq_t a_nm3 = (seq_t)25;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)85;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)286;  /* a(n-1) */
    seq_t cur;                 /* a(n) */

    for (idx_t i = 6; i <= n; ++i) {
        cur = (seq_t)3 * a_nm1
              + a_nm2
              + (seq_t)2 * a_nm3
              - (seq_t)2 * a_nm4
              - (seq_t)4 * a_nm5;

        a_nm5 = a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}
