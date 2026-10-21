/*
 * A259042  Period 8 sequence [0, 1, 1, 1, 2, 1, 1, 1, ...].
 * https://oeis.org/A259042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 9453, u16 n <= 9453, u32 n <= 9453, u64 n <= 9453,
 *            u128 n <= 9453, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence A259042: a(n) = {0,1,1,1,2,1,1,1} repeating every 8. */
seq_t a(idx_t n)
{
    static const seq_t tbl[8] = {0, 1, 1, 1, 2, 1, 1, 1};
    return tbl[n & 7];
}
