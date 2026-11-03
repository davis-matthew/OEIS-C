/*
 * A286507  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 190", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A286507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(floor(n/2))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 126 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* floor division by 2 */
    return pow10(n >> 1);
}
