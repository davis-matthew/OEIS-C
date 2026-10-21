/*
 * A288700  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 494", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A288700: a(0)=1; a(1)=2; a(2)=0; a(n)=2^n for odd n≥3; a(n)=2^(n‑1) for even n≥4. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)0;
    if (n & (idx_t)1)              /* odd */
        return pow2(n);
    else                            /* even */
        return pow2(n - (idx_t)1);
}
