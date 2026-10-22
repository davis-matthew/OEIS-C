/*
 * A381054  a(n) is the least k such that floor(sqrt(n*k/d(n*k))) -
 *          floor(sqrt(d(n*k))) = 1, where d(k) is the largest divisor of k
 *          which is <= sqrt(k).
 * https://oeis.org/A381054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least k such that floor(sqrt(n*k/d(n*k))) -
 *            floor(sqrt(d(n*k))) = 1 where d(m) is the largest divisor of m
 *            which is <= sqrt(m)
 * Range:     u8 n <= 15, u16 n <= 415, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Search for the smallest k satisfying the condition. */
    for (idx_t k = 1; ; ++k) {
        seq_t m = (seq_t)n * (seq_t)k;            /* m = n*k   */
        seq_t s = floor_sqrt(m);                  /* floor(sqrt(m)) */

        /* Find the largest divisor of m that is <= s.  */
        seq_t d = 1;                              /* at least 1 */
        idx_t t = (idx_t)s;                       /* the loop counter, fits in idx_t */

        /* If m is a perfect square we can skip the loop. */
        if (m % (seq_t)t != 0) {
            while (t > 1) {                       /* t will never reach 0 due to break */
                --t;
                if (m % (seq_t)t == 0) {
                    d = (seq_t)t;
                    break;
                }
            }
        } else {
            d = (seq_t)t;                         /* m is a square, d = sqrt(m) */
        }

        seq_t lhs = floor_sqrt(m / d);
        seq_t rhs = floor_sqrt(d);

        if (lhs - rhs == 1) {
            return (seq_t)k;                      /* k is the desired a(n) */
        }
    }
}
