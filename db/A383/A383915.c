/*
 * A383915  Number of points enclosed by the unique circle that goes through
 *          the 8 points (-n, 0), (-n, 1), (0, n+1), (1, n+1), (n+1, 1), (n+1,
 *          0), (1, -n), (0, -n).
 * https://oeis.org/A383915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * Sum_{i=1..n} ceil(sqrt(n^2+n+1/2-(i-1/2)^2)-1/2)
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Converted formula: V = n^2 + n - i(i-1) is integer.
       ceil(sqrt(V)-1/2) = round(sqrt(V)) =
          floor_sqrt(V) + (V >= floor_sqrt(V)^2 + floor_sqrt(V) + 1 ? 1 : 0) */
    seq_t n_seq = (seq_t)n;
    seq_t N2 = n_seq * n_seq + n_seq;   /* n^2 + n */
    seq_t sum = 0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t V = N2 - ii * (ii - 1);                 /* V = n^2 + n - i(i-1) */
        seq_t r = floor_sqrt(V);                      /* floor sqrt of V */
        seq_t thresh = r * r + r + (seq_t)1;          /* r^2 + r + 1 */
        seq_t term = r + (V >= thresh ? (seq_t)1 : (seq_t)0);
        sum += term;
    }

    return sum * (seq_t)4;
}
