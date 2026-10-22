/*
 * A335873  Total number of points in all permutations of [n] that are fixed
 *          or reflected.
 * https://oeis.org/A335873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n! - (n mod 2)*(n-1)! (a(0) = 0)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t fact_n = factorial(n);
    seq_t fact_n_minus1 = factorial(n - 1);
    seq_t res = fact_n * 2;
    if ((n & 1) != 0) res -= fact_n_minus1;
    return res;
}
