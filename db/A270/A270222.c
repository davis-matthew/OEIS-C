/*
 * A270222  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 131", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1 * a(n) = 1 + 3*4^n - 2^(n+2) for n >= 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* 4^n and 2^(n+2) may be huge; seq_pow handles arbitrary precision */
    seq_t pow4 = seq_pow((seq_t)4, n);
    seq_t pow2 = seq_pow((seq_t)2, n + 2);

    seq_t result = (seq_t)1 + 3 * pow4;
    result -= pow2;
    return result;
}
