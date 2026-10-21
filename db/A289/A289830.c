/*
 * A289830  a(n) satisfies the equation n/(n-1) + a(n)/n! = H(n), where H(n)
 *          is the n-th harmonic number.
 * https://oeis.org/A289830
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * (H(n) - n/(n-1))
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Computes a(n) for n >= 2.
   Returns the value according to the OEIS definition a(n) = n! * (H(n) - n/(n-1)). */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;         /* offset starts at 2; behaviour below is undefined */
    seq_t cur_a = (seq_t)-1;            /* value of a(2) */
    seq_t cur_f = (seq_t)1;             /* (0)! for n = 2 */

    for (idx_t cur = 2; cur < n; ++cur) {
        /* term1 = (cur + 1) * cur_a  */
        seq_t term1 = (seq_t)(cur + 1) * cur_a;

        /* term2 = (cur*cur + 1) * cur_f  */
        seq_t sq1 = (seq_t)cur * (seq_t)cur + (seq_t)1;
        seq_t term2 = sq1 * cur_f;

        cur_a = term1 + term2;           /* a(cur+1) */
        cur_f *= (seq_t)(cur - 1);        /* update (cur+1-2)! = (cur-1)! */
    }

    return cur_a;
}
