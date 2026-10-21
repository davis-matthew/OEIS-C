/*
 * A269708  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 14", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*3^(n-2)+4^n for n>1, a(0)=1, a(1)=5, a(2)=20
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)20;
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 2));
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    return ((seq_t)4 * pow3) + pow4;
}
