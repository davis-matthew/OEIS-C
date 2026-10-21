/*
 * A281051  Number of n X 3 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) - 4*a(n-3) + 8*a(n-4) (n > 6) Initial
 *            values: a(1)=0, a(2)=6, a(3)=68, a(4)=376, a(5)=1492, a(6)=4988
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 21, u64 n <= 50, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
/* The types seq_t and idx_t are defined by the build system.
   seq_t supports standard arithmetic operators (+ - *). */

seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n < 1, return 0 */
        return (seq_t)0;

    /* Base values for 1 ≤ n ≤ 6 */
    static const seq_t base[6] = { (seq_t)0, (seq_t)6, (seq_t)68,
                                   (seq_t)376, (seq_t)1492, (seq_t)4988 };

    if (n <= 6)
        return base[n-1];

    /* Sliding window for the recurrence */
    seq_t a_nm4 = base[2]; /* a(3)   */
    seq_t a_nm3 = base[3]; /* a(4)   */
    seq_t a_nm2 = base[4]; /* a(5)   */
    seq_t a_nm1 = base[5]; /* a(6)   */
    seq_t ai    = (seq_t)0; /* placeholder for a(n) */

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute 5*a_nm1 - 6*a_nm2 - 4*a_nm3 + 8*a_nm4 using seq_t arithmetic */
        ai = (seq_t)5 * a_nm1 - (seq_t)6 * a_nm2 - (seq_t)4 * a_nm3 + (seq_t)8 * a_nm4;
        /* Shift the window */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = ai;
    }
    return ai;
}
