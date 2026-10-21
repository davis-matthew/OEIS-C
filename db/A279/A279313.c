/*
 * A279313  Period 14 zigzag sequence: repeat [0,1,2,3,4,5,6,7,6,5,4,3,2,1].
 * https://oeis.org/A279313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n % 14, if the quotient is ≤ 7; otherwise a(n) = 14 - (n
 *            % 14)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the period‑14 zigzag sequence: 0,1,2,3,4,5,6,7,6,5,4,3,2,1 repeat. */

seq_t a(idx_t n)
{
    idx_t r = n % 14;            /* residue modulo 14 */
    if (r <= 7)
        return (seq_t)r;        /* 0…7 */
    else
        return (seq_t)(14 - r); /* 6…1 */
}
