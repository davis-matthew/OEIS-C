/*
 * A303150  Total domination number of the n-transposition graph.
 * https://oeis.org/A303150
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2 for n = 2,3; a(4) = 6; otherwise a(n) = 4^(n‑3)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n <= 3)
        return (seq_t)2;
    if (n == 4)
        return (seq_t)6;
    return seq_pow((seq_t)4, n - 3);
}
