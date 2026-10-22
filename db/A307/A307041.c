/*
 * A307041  a(n) = Sum_{k=0..floor(n/7)} (-1)^k*binomial(n,7*k).
 * https://oeis.org/A307041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/7)} (-1)^k*binomial(n,7*k) /* Initial
 *            values: a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=1 * Recurrence:
 *            a(n)=7*a(n-1)-21*a(n-2)+35*a(n-3)-35*a(n-4)+21*a(n-5)-7*a(n-6)
 *            for n>6
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 36, i64 n <= 67, i128 n <= 134,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Handle the small indices directly. */
    if (n <= 6)
        return (seq_t)1;

    /* A small circular buffer holding the last 7 computed values.
       Index order corresponds to consecutive a(i) values:
         buf[0] = a(i-6), buf[1] = a(i-5), ..., buf[6] = a(i) */
    seq_t buf[7];
    for (int j = 0; j < 7; ++j)
        buf[j] = (seq_t)1;   /* a(0)..a(6) are all 1 */

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute a(i) using the recurrence and previous 6 values. */
        seq_t val = mul_small(buf[6], 7);          /* a(i-1) */
        val -= mul_small(buf[5], 21);              /* a(i-2) */
        val += mul_small(buf[4], 35);              /* a(i-3) */
        val -= mul_small(buf[3], 35);              /* a(i-4) */
        val += mul_small(buf[2], 21);              /* a(i-5) */
        val -= mul_small(buf[1], 7);               /* a(i-6) */

        /* Shift the buffer left to make room for the new value. */
        for (int j = 0; j < 6; ++j)
            buf[j] = buf[j + 1];
        buf[6] = val;                              /* now buf[6] == a(i) */
    }

    /* After the loop, buf[6] holds a(n). */
    return buf[6];
}
