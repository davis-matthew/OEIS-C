/*
 * A267061  Coefficient of x^2 in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(2)+sqrt(3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A267061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 34*a(n-1) + 714*a(n-2) - 4641*a(n-3) - 12376*a(n-4) +
 *            12376*a(n-5) + 4641*a(n-6) - 714*a(n-7) - 34*a(n-8) + a(n-9)
 *            Initial values: a(0) = -50, a(1) = 2498, a(2) = 173262, a(3) =
 *            7783550, a(4) = 376636138, a(5) = 17527857350, a(6) =
 *            826628182158, a(7) = 38778106729442, a(8) = 1822757247598510
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 4, i64 n <= 10, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* First 9 terms of the sequence */
    static const seq_t init[9] = {
        -(seq_t)50,
        (seq_t)2498,
        (seq_t)173262,
        (seq_t)7783550,
        (seq_t)376636138,
        (seq_t)17527857350,
        (seq_t)826628182158,
        (seq_t)38778106729442,
        (seq_t)1822757247598510
    };

    if (n < 9) {
        return init[n];
    }

    /* Sliding window of the last 9 terms */
    seq_t buf[9];
    for (idx_t i = 0; i < 9; ++i) {
        buf[i] = init[i];
    }

    seq_t next;
    for (idx_t k = 9; k <= n; ++k) {
        next = (seq_t)34 * buf[8]
             + (seq_t)714 * buf[7]
             + (seq_t)(-4641) * buf[6]
             + (seq_t)(-12376) * buf[5]
             + (seq_t)12376 * buf[4]
             + (seq_t)4641 * buf[3]
             + (seq_t)(-714) * buf[2]
             + (seq_t)(-34) * buf[1]
             + buf[0];

        /* Shift the window left by one position */
        for (idx_t j = 0; j < 8; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[8] = next;
    }

    return buf[8];
}
