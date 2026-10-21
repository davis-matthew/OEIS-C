/*
 * A273848  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 969", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*4^n - 4*2^n + 1 (valid for n > 2); a(0)=1, a(1)=4,
 *            a(2)=45
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 45;

    seq_t pow4_n = pow4(n);    /* 4^n */
    seq_t pow2_n = pow2(n);    /* 2^n */

    return ((seq_t)4) * pow4_n - ((seq_t)4) * pow2_n + 1;
}
