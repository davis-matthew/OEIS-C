/*
 * A324964  a(n) = A000139(n) mod 2; Characteristic function of odd fibbinary
 *          numbers (A022341).
 * https://oeis.org/A324964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9055 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 iff the binary representation of n ends in 1 and contains no
   consecutive 1s; otherwise a(n) = 0. */
seq_t a(idx_t n)
{
    /* last bit must be 1 */
    if ((n & 1) == 0)
        return (seq_t)0;

    /* check for adjacent 1s: n & (n >> 1) will be non‑zero if any two
       consecutive bits are both 1. */
    if ((n & (n >> 1)) != 0)
        return (seq_t)0;

    return (seq_t)1;
}
