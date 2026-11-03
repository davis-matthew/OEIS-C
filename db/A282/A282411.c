/*
 * A282411  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 467", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1; for n>=2, a(n) = (10^(n+1)-1)/9 if n is even,
 *            otherwise a(n)=0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 116 (2026-11-01)
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
    /* Handle the two special initial terms */
    if (n < 2)
        return 1;

    /* Odd indices (n>=3) give 0 */
    if (n & 1U)
        return 0;

    /* Even indices: compute (10^(n+1)-1)/9 */
    seq_t val = pow10(n + 1);
    val -= 1;
    val /= 9;
    return val;
}
