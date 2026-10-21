/*
 * A280853  Number of n X 2 0..2 arrays with no element equal to more than one
 *          of its king-move neighbors and with new values introduced in order
 *          0 sequentially upwards.
 * https://oeis.org/A280853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + 8*a(n-3) - 8*a(n-4) for n>5 Initial
 *            values: a(1)=2, a(2)=9, a(3)=29, a(4)=110, a(5)=442
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* idx_t and seq_t are defined by the build */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1 only.  Return 0 for n < 1. */
    if (n < 1) return 0;

    /* Handle the first five terms directly.  Using a static array
       keeps the values in a single definition and avoids magic numbers. */
    static const seq_t init[6] = {0, 2, 9, 29, 110, 442};  /* init[1] … init[5] */
    if (n <= 5) return init[n];

    /* Use four variables to hold the previous four terms.
       For n = 6 the previous values are:
         a(n-1) = a(5) = 442
         a(n-2) = a(4) = 110
         a(n-3) = a(3) = 29
         a(n-4) = a(2) = 9     */
    seq_t a_nm1 = 442;  /* a(i-1)  */
    seq_t a_nm2 = 110;  /* a(i-2)  */
    seq_t a_nm3 = 29;   /* a(i-3)  */
    seq_t a_nm4 = 9;    /* a(i-4)  */

    for (idx_t i = 6; i <= n; ++i) {
        /* Compute a(i) from the recurrence.  All operations are in seq_t. */
        seq_t new_val = 4 * a_nm1 - 2 * a_nm2 + 8 * a_nm3 - 8 * a_nm4;

        /* Shift the window: the new term becomes the next a_nm1. */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = new_val;
    }

    /* After the loop a_nm1 holds a(n). */
    return a_nm1;
}
