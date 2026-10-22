/*
 * A398470  Number of increasing 3-term arithmetic progressions in an n^3
 *          cube.
 * https://oeis.org/A398470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n even ? (n^6-8*n^3)/16 : ((n^2+1)^3-8*n^3)/16)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute A398470: number of increasing 3‑term arithmetic progressions in an n³ cube */
seq_t a(idx_t n)
{
    /* n >= 1 for the OEIS offset. Non‑positive indices return 0 */
    if (n < 1)
        return 0;

    seq_t N = (seq_t)n;
    if ((n & 1ULL) == 0) {         /* n even */
        seq_t n3 = N * N * N;
        seq_t n6 = n3 * n3;
        return (n6 - (seq_t)8 * n3) / (seq_t)16;
    } else {                       /* n odd */
        seq_t n2 = N * N;
        seq_t n2p1 = n2 + (seq_t)1;
        seq_t n2p1_cub = n2p1 * n2p1 * n2p1;
        seq_t n3 = N * N * N;
        return (n2p1_cub - (seq_t)8 * n3) / (seq_t)16;
    }
}
