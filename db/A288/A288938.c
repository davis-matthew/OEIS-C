/*
 * A288938  Positions of 1 in A288936; complement of A288937.
 * https://oeis.org/A288938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + k, where k is the number of zero positions (A288937)
 *            below a(n); A288937(j) = 1,2,4,5 for j<=4, else j*(j+1)/2 - 3*j
 *            + 8
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9505 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A288938_zero(seq_t j)
{
    if (j == 1) return 1;
    if (j == 2) return 2;
    if (j == 3) return 4;
    if (j == 4) return 5;
    return j * (j + 1) / 2 - 3 * j + 8;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t k = 0;
    while (A288938_zero(k + 1) <= N + k)
        k++;
    return N + k;
}
