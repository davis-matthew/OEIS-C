/*
 * A269160  Formula for Wolfram's Rule 30 cellular automaton: a(n) = n XOR (2n
 *          OR 4n).
 * https://oeis.org/A269160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n XOR (2n OR 4n)
 * Range:     u8 n <= 63, u16 n <= 9368, u32 n <= 9368, u64 n <= 9368,
 *            u128 n <= 9368, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                /* cast index to seq_t */
    seq_t two_n  = sn * (seq_t)2;       /* 2n */
    seq_t four_n = sn * (seq_t)4;       /* 4n */
    return sn ^ (two_n | four_n);      /* n XOR (2n OR 4n) */
}
