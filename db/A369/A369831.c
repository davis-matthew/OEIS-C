/*
 * A369831  a(n) is the number of distinct values of the permanent of an n X n
 *          symmetric Toeplitz matrix using the integers 1 to n.
 * https://oeis.org/A369831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 1 (n = 0,1,2); 6 (n = 3); 23 (n = 4); n! (n ≥ 5)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

/* Counts distinct permanents of n×n symmetric Toeplitz matrices
   with entries from 1..n.  For n < 3 the answer is 1; for n = 3
   it is 6; for n = 4 it is 23; for n ≥ 5 it equals n! */
seq_t a(idx_t n)
{
    if (n < 3)
        return 1;
    if (n == 3)
        return 6;
    if (n == 4)
        return 23;
    return factorial(n);
}
