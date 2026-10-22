/*
 * A359696  a(n) is the number of points with integer coordinates located
 *          between the x-axis and the graph of the function y = n^3 / (n^2 +
 *          x^2).
 * https://oeis.org/A359696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 2*Sum_{y=1}^{n-1} floor(sqrt(n^3 / y - n^2))
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 426, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses integer arithmetic only. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* n is unsigned. */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t sum = 0;
    for (idx_t y = 1; y < n; ++y) {
        seq_t yt = (seq_t)y;
        seq_t num = n3 - n2 * yt;      /* n^3 - n^2*y */
        seq_t t   = num / yt;          /* floor(n^3 / y - n^2) as integer */
        seq_t s   = floor_sqrt(t);     /* floor(sqrt(t)) */
        sum += s;
    }
    return nn + 2 * sum;
}
