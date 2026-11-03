/*
 * A357678  Numbers k equal to the integer log of the sum of k and its digit
 *          reversal.
 * https://oeis.org/A357678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, bref, more, hard
 * Formula:   a(n) = 10^(n-1) + 7
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 3 terms through n = 3 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* All terms of A357678 are of the form 10^m + 7. */

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
        return 0;            /* index below offset (not used) */
    return pow10(n - 1) + 7;
}
