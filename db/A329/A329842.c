/*
 * A329842  Beatty sequence for (13+sqrt(109))/10.
 * https://oeis.org/A329842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(13+sqrt(109))/10)
 * Range:     u8 none, u16 n <= 12, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Handle the offset.  For n == 0 the sequence is undefined, but
       this guard prevents a bogus calculation if called with 0. */
    if (n == 0)
        return 0;

    /* Compute floor(n * sqrt(109)) as floor_sqrt(109 * n^2). */
    seq_t nn  = (seq_t)n;
    nn *= nn;                      /* n^2  */
    seq_t mul = nn * (seq_t)109;   /* 109 * n^2 */
    seq_t sqh = floor_sqrt(mul);   /* floor(sqrt(109 * n^2)) */

    /* a(n) = (13*n + floor(n*sqrt(109))) / 10 */
    seq_t val = ((seq_t)13 * (seq_t)n + sqh) / (seq_t)10;
    return val;
}
