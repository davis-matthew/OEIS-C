/*
 * A267811  Binary representation of the n-th iteration of the "Rule 217"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n=0,1; otherwise a(n) = ((10^n − 1)/9) * (10^(n+1)
 *            + 1) // binary string with n ones, a 0, then n ones
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t pow10n   = pow_seq((seq_t)10, (int)n);        /* 10^n   */
    seq_t ones     = (pow10n - (seq_t)1) / (seq_t)9;    /* (10^n-1)/9  */
    seq_t pow10n1  = pow_seq((seq_t)10, (int)(n + 1)); /* 10^(n+1) */
    seq_t part2    = pow10n1 + (seq_t)1;               /* 10^(n+1)+1 */
    return ones * part2;
}
