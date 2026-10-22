/*
 * A372847  Number of unit squares enclosed by a circle of radius n with an
 *          even number of rows and the maximum number of squares in each row.
 * https://oeis.org/A372847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Sum_{k=1..n-1} floor(2*sqrt(n^2 - k^2))
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t N = n;                 /* use idx_t for intermediate arithmetic */
    for (idx_t k = 1; k < n; ++k) {
        idx_t t = N * N - k * k;           /* n^2 - k^2 as idx_t to avoid overflow */
        seq_t t_seq = (seq_t)t;            /* cast to seq_t for sqrt */
        seq_t s = floor_sqrt(t_seq);       /* floor(sqrt(t)) */
        seq_t threshold = s * s + s + 1;   /* ceil(s^2 + s + 0.25) */
        seq_t val = (t >= threshold) ? (2 * s + 1) : (2 * s);
        sum += val;
    }
    return sum << 1;                  /* 2 * sum */
}
