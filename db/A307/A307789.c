/*
 * A307789  Number of valid hook configurations of permutations of [n] that
 *          avoid the patterns 231 and 1243.
 * https://oeis.org/A307789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=a(2)=1;
   for n>=3:  a(n) = 2*a(n-1) + (3*(n-3)*a(n-2))/(n-1) */
   
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;

    seq_t a_prev_prev = (seq_t)1;   /* a(1) */
    seq_t a_prev      = (seq_t)1;   /* a(2) */
    seq_t a_cur = (seq_t)0;

    for (idx_t m = 3; m <= n; ++m) {
        /* Compute the fraction term: (3*(m-3)*a_prev_prev)/(m-1) */
        seq_t num = (seq_t)3 * (seq_t)(m - 3);
        num = num * a_prev_prev;                 /* safe: intermediate fits for n where result fits seq_t */
        seq_t second = num / (m - 1);

        a_cur = (seq_t)2 * a_prev + second;

        a_prev_prev = a_prev;
        a_prev = a_cur;
    }
    return a_cur;
}
