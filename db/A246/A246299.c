/*
 * A246299  Numbers k such that sin(k) > sin(k+1) > sin(k+2) > sin(k+3) <
 *          sin(k+4).
 * https://oeis.org/A246299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k (k >= 0) such that sin(k) > sin(k+1) >
 *            sin(k+2) > sin(k+3) < sin(k+4)
 * Range:     u8 n <= 41, u16 n <= 987, u32 n <= 987, u64 n <= 987,
 *            u128 n <= 987, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches for the required k by scanning k
   consecutively and evaluating the inequality with long double
   precision. */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1, but guard against 0 */

    uint64_t k      = 0;   /* candidate value */
    idx_t    cnt    = 0;   /* how many values found so far */

    while (cnt < n) {
        /* compute the sine values with long double precision */
        long double s0 = sinl((long double)k);
        long double s1 = sinl((long double)(k + 1));
        long double s2 = sinl((long double)(k + 2));
        long double s3 = sinl((long double)(k + 3));
        long double s4 = sinl((long double)(k + 4));

        /* check the strict inequalities */
        if (s0 > s1 && s1 > s2 && s2 > s3 && s3 < s4) {
            cnt++;
            if (cnt == n) {
                return (seq_t)k;
            }
        }
        k++;  /* next candidate */
    }

    /* theoretically unreachable */
    return (seq_t)0;
}
