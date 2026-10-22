/*
 * A359096  The sum of the numbers on the perimeter of the n X n diamond
 *          frame, located at the top of the numerical pyramid containing the
 *          positive integers in natural order.
 * https://oeis.org/A359096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(1) = 1, for n > 1 a(n) = (8*n^3 - 12*n^2 + 13*n - 9)/3
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 1171, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    seq_t N = (seq_t)n;
    seq_t num = 8 * N * N * N - 12 * N * N + 13 * N - (seq_t)9;
    return num / (seq_t)3;
}
