/*
 * A260866  Base-16 representation of a(n) is the concatenation of the base-16
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the integer whose base‑16 representation is the *
 *            concatenation of the base‑16 representations of 1,2,…,n,n‑1,…,1.
 *            * (a(0)=0). * * Implementation detail: build the number digit by
 *            digit in base‑16. * For each integer x (1 ≤ x ≤ n) we repeatedly
 *            take its most * significant hex digit, multip...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Append the base‑16 digits of x to the current accumulator acc.
 * Returns the new accumulator value. */
static inline seq_t
append_hex_digits(seq_t acc, idx_t x)
{
    /* Find the highest 4‑bit block of x. */
    idx_t shift = 0;
    idx_t tmp = x;
    /* Count how many 4‑bit blocks x has. */
    while (tmp >> 4) {          /* tmp >>= 4; */
        tmp >>= 4;
        shift += 4;
    }
    if (shift == 0) {
        /* x is in [1,15], single hex digit. */
        return acc * 16 + (x & 0xF);
    }
    /* Process all hex digits from most significant to least. */
    for (; ; /*no init*/ ) {
        idx_t digit = (x >> shift) & 0xF;
        acc = acc * 16 + digit;
        if (shift == 0)
            break;
        shift -= 4;
    }
    return acc;
}

seq_t a(idx_t n)
{
    seq_t acc = 0;
    if (n == 0)
        return acc;

    /* Forward part: 1 .. n */
    for (idx_t i = 1; i <= n; ++i)
        acc = append_hex_digits(acc, i);

    /* Backward part: n‑1 .. 1 */
    if (n > 1)
        for (idx_t i = n - 1; i >= 1; --i)
            acc = append_hex_digits(acc, i);

    return acc;
}
