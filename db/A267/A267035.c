/*
 * A267035  Binary representation of the n-th iteration of the "Rule 85"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(2n) if n is even * a(n) = (10^(2n) - 1)/9 if n is odd
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 990 (2026-11-01)
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
    /* exponent 2n */
    idx_t exp = (idx_t)n + n;      /* safe addition for uint64_t */
    seq_t p = pow10(exp);          /* 10^(2n) */

    /* if n is odd, adjust for 111…1 */
    if (n & 1u)
    {
        seq_t t = p - 1;
        return t / 9;
    }
    return p;
}
