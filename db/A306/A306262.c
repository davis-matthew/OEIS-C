/*
 * A306262  Difference between maximum and minimum sum of products of
 *          successive pairs in permutations of [n].
 * https://oeis.org/A306262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0 a(n) = (2*n^3 + 6*n^2 - 26*n + 12 + 6*(n%2)) / 12 for n
 *            ≥ 1
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation avoids unsigned underflow by first
   computing a positive intermediate value that is at least
   26*n, then subtracting.  All arithmetic is performed in
   seq_t to preserve arbitrary‑precision requirements. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t n_sq = nn * nn;
    seq_t n3 = n_sq * nn;

    /* Part1 = 2*n^3, Part2 = 6*n^2 */
    seq_t part1 = (seq_t)2 * n3;
    seq_t part2 = (seq_t)6 * n_sq;

    /* partA = 2*n^3 + 6*n^2 + 12 + 6*(n%2)   (always ≥ 26*n for n≥1) */
    seq_t partA = part1 + part2 + 12;
    partA += (nn & 1) * (seq_t)6;   /* add 6 if n is odd */

    /* subtract 26*n */
    seq_t partB = (seq_t)26 * nn;
    seq_t N = partA - partB;

    /* division by 12 (exact) */
    return N / 12;
}
