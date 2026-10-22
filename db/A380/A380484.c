/*
 * A380484  a(n) = 1 if n is congruent to {0, 1, 2, 3, 4, 5} mod 30, otherwise
 *          0.
 * https://oeis.org/A380484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 if n mod 30 is in {0,1,2,3,4,5}; otherwise a(n) = 0
 * Range:     u8 n <= 9293, u16 n <= 9293, u32 n <= 9293, u64 n <= 9293,
 *            u128 n <= 9293, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The modulus fits in idx_t; the result of the comparison is 0 or 1. */
    return ( (n % 30ULL) <= 5ULL ) ? 1 : 0;
}
