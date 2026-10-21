/*
 * A270825  a(n) = Sum_{i=0..n} (-1)^floor(i/2)*floor(sqrt(i)).
 * https://oeis.org/A270825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{i=0..n} (-1)^⌊i/2⌋ ⌊√i⌋
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
/* _floor_isqrt_u64 is provided by the test harness; it returns floor(√x). */
#ifndef _floor_isqrt_u64
idx_t _floor_isqrt_u64(idx_t n);
#endif

seq_t a(idx_t n) {
    seq_t result = 0;
    /* Use incremental square root calculation */
    idx_t next_square = 1;   /* next perfect square threshold */
    idx_t cur_sqrt   = 0;    /* current floor sqrt value */
    for (idx_t i = 0; i <= n; ++i) {
        if (i == next_square) {
            ++cur_sqrt;
            /* update next perfect square; be careful of overflow */
            next_square = (idx_t)(cur_sqrt + 1) * (idx_t)(cur_sqrt + 1);
        }
        /* sign pattern: + for i%4 < 2, - otherwise */
        seq_t sign = ((i & 2) == 0) ? 1 : -1;
        result += sign * (seq_t)cur_sqrt;
    }
    return result;
}
