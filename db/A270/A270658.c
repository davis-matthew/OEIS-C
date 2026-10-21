/*
 * A270658  a(3n)=a(n), a(3n+1)=a(3n)+a(3(n+1)), a(3n+2)=a(3n)-a(3(n+1));
 *          a(0)=0, a(1)=1.
 * https://oeis.org/A270658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(3n) = a(n), a(3n+1) = a(n)+a(n+1), a(3n+2) = a(n)-a(n+1);
 *            a(0)=0, a(1)=1
 * Range:     i8 n <= 9667, i16 n <= 9667, i32 n <= 9667, i64 n <= 9667,
 *            i128 n <= 9667, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t a_rec(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    idx_t m = n / 3;
    idx_t r = n % 3;
    if (r == 0) {
        return a_rec(m);
    } else if (r == 1) {
        return a_rec(m) + a_rec(m + 1);
    } else {    /* r == 2 */
        return a_rec(m) - a_rec(m + 1);
    }
}

seq_t a(idx_t n) {
    return a_rec(n);
}
