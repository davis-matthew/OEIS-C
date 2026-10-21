/*
 * A265720  Binary representation of the n-th iteration of the "Rule 1"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A265720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n if n even * a(n) = (10*100^n - 999*10^(n-1) - 1)/9
 *            if n odd * * Sequence A265720: Binary representation of the n-th
 *            iteration of * Rule 1 elementary cellular automaton starting
 *            with a single ON cell
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Even indices */
    if ((n & 1) == 0) {
        return pow10(n);
    }

    /* Odd indices: compute (10*100^n - 999*10^(n-1) - 1)/9 */
    seq_t pow10_2n  = pow10(2 * n);       /* 100^n = 10^(2n)   */
    seq_t pow10_n1 = pow10(n - 1);        /* 10^(n-1)          */

    seq_t num = 10 * pow10_2n - 999 * pow10_n1 - 1;

    return num / 9;
}
