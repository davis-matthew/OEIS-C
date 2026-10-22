/*
 * A398934  Least positive integer that, for every k with 1 <= k <= n, is the
 *          volume of a k-dimensional rectangular box with lattice-point
 *          vertices, integral edge lengths, and every edge having nonzero
 *          components in all k coordinate directions.
 * https://oeis.org/A398934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1) = 1 a(2) = 25 a(3) = 675 a(n) = 25 * 7^(n-1) for all n ≥ 4
 *            (The minimal common volume grows by a factor of 7 after the
 *            third term.)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"   /* seq_pow(seq_t, idx_t) */

/* Compute A398934(n) – the least common volume of k‑dimensional
   lattice boxes with all edge components non‑zero, for every
   k with 1 ≤ k ≤ n. */
seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 25;
    if (n == 3) return 675;

    /* For n ≥ 4: 25 * 7^(n-1) */
    seq_t pow7 = seq_pow((seq_t)7, n - 1);
    return 25 * pow7;
}
