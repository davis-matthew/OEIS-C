/*
 * A277105  a(n) = (27*3^n - 63)/2.
 * https://oeis.org/A277105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (27*3^n - 63)/2
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For safety we return 0 for n = 0. */
    if (n == 0ULL) return (seq_t)0;

    /* Compute 3^n.  The helper pow_seq performs repeated squaring in
       exact arithmetic (big‑int for arbitrary precision builds). */
    seq_t pow3 = pow_seq((seq_t)3, (int)n);

    /* Apply the formula, using integer arithmetic only. */
    seq_t result = ((seq_t)27 * pow3 - (seq_t)63) / (seq_t)2;
    return result;
}
