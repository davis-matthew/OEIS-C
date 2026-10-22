/*
 * A352675  Beatty sequence for (3/13)(4+sqrt(3)).
 * https://oeis.org/A352675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((12*n + floor_sqrt(27*n^2))/13)
 * Range:     u8 n <= 1, u16 n <= 24, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Beatty sequence for (3/13)(4+sqrt(3)), offset 1. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but safe guard */

    seq_t N   = (seq_t)n;               /* convert index to seq_t */
    seq_t n2  = N * N;                  /* n^2 */
    seq_t v   = 27 * n2;                /* 27*n^2 */
    seq_t s   = floor_sqrt(v);          /* floor(sqrt(27*n^2)) = floor(3*n*sqrt(3)) */
    seq_t num = 12 * N + s;             /* 12*n + floor(3*n*sqrt(3)) */
    seq_t ans = num / 13;               /* floor( (12*n + floor(3*n*sqrt(3))) / 13 ) */

    return ans;
}
