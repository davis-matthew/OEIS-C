/*
 * A266299  Binary representation of the n-th iteration of the "Rule 14"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, a(n)=11*10^(2*n-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 979 (2026-11-01)
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
    if (n == 0)
        return (seq_t)1;
    /* exponent 2*n-1 is safe for the required ranges */
    idx_t exp = 2 * n - 1;
    seq_t x = pow10(exp);
    return x * (seq_t)11;
}
