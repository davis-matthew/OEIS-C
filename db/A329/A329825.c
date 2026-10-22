/*
 * A329825  Beatty sequence for (3+sqrt(17))/4.
 * https://oeis.org/A329825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((3 + sqrt(17))/4) * n)
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 7905, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* use integer arithmetic only */
    seq_t ns = (seq_t)n;              /* n as seq_t */
    seq_t ns2 = ns * ns;              /* n^2  */
    seq_t v   = (seq_t)17 * ns2;      /* 17*n^2  */
    seq_t m   = floor_sqrt(v);        /* floor(n*sqrt(17)) */
    seq_t k   = (seq_t)3 * ns;        /* 3*n  */
    seq_t sum = k + m;                /* 3n + floor(n*sqrt(17)) */
    return sum / (seq_t)4;            /* floor((3n + floor(n*sqrt(17))) / 4) */
}
