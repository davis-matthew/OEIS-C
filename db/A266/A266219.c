/*
 * A266219  Binary representation of the middle column of the "Rule 7"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + (n mod 2), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence for the binary representation of the
   middle column of Rule 7, yielding decimal numbers composed of digits
   0 and 1. */
seq_t a(idx_t n)
{
    seq_t v = 1;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        v = v * (seq_t)10 + (seq_t)(i & 1);
    }
    return v;
}
