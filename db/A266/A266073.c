/*
 * A266073  Number of OFF (white) cells in the n-th iteration of the "Rule 3"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1)^n*n + n - (-1)^n + 1 (equivalently a(n)=2*n if n
 *            even, else 2)
 * Range:     u8 n <= 127, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n) is even for all n, except constant 2 when n is odd */
    if ((n & 1) == 0)            /* even n */
        return (seq_t)n * 2;     /* 2*n fits in seq_t; wraps modulo 2^BIT if too large */
    else
        return 2;                /* odd n */
}
