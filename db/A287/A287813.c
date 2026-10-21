/*
 * A287813  Number of octonary sequences of length n such that no two
 *          consecutive terms have distance 2.
 * https://oeis.org/A287813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=8, a(2)=52; * For n>2 a(n)=7*a(n-1)-3*a(n-2) (OEIS
 *            A287813)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the initial conditions directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)52;

    /* For n>=3, compute iteratively using the recurrence. */
    seq_t prev1 = (seq_t)52; /* a(2)   */
    seq_t prev2 = (seq_t)8;  /* a(1)   */
    seq_t curr  = 0;

    for (idx_t i = 3; i <= n; ++i) {
        curr = prev1 * (seq_t)7 - prev2 * (seq_t)3;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1; /* a(n) */
}
