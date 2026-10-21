/*
 * A276865  First differences of the Beatty sequence A003512 for 2 + sqrt(3).
 * https://oeis.org/A276865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(2+sqrt(3))) - floor((n-1)*(2+sqrt(3))) = 2 +
 *            floor(sqrt(3*n^2)) - floor(sqrt(3*(n-1)^2)) (here sqrt() denotes
 *            the floor of the square root)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t nseq   = (seq_t)n;
    seq_t n1seq  = (seq_t)(n - 1);

    /* floor(sqrt(3*n^2)) */
    seq_t sq_cur = floor_sqrt((seq_t)3 * nseq * nseq);

    /* floor(sqrt(3*(n-1)^2)) */
    seq_t sq_prev = floor_sqrt((seq_t)3 * n1seq * n1seq);

    return (seq_t)2 + sq_cur - sq_prev;
}
