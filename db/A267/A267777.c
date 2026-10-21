/*
 * A267777  Binary representation of the n-th iteration of the "Rule 209"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = ((10^(2n-2) – 1) / 9) * 1000 + 1
 *            (decimal number with 2n–2 leading 1's followed by 001)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)1;

    /* number of leading 1's */
    idx_t r = (idx_t)(2 * n - 2);

    /* 10^r */
    seq_t pow_r = pow10(r);

    /* (10^r - 1) / 9  →  r ones in decimal */
    seq_t ones = (pow_r - (seq_t)1) / (seq_t)9;

    /* append "...001" by shifting left 3 decimal places and adding 1 */
    seq_t result = ones * (seq_t)1000 + (seq_t)1;
    return result;
}
