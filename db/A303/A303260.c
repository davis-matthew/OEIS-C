/*
 * A303260  Determinant of n X n matrix A[i,j] = (j - i - 1 mod n) + [i=j],
 *          i.e., the circulant having (n, 0, 1, ..., n-2) as first row.
 * https://oeis.org/A303260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + n(n-1)/2) * ((n+1)^n - 1) / n^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t sn = (seq_t)n;
    seq_t part1 = (seq_t)1 + sn * (sn - (seq_t)1) / (seq_t)2;
    seq_t pow = pow_seq(sn + (seq_t)1, (int)n);
    seq_t part2 = (pow - (seq_t)1) / (sn * sn);
    return part1 * part2;
}
