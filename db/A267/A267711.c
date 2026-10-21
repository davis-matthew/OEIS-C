/*
 * A267711  Numbers k such that k mod 3 = k mod 5.
 * https://oeis.org/A267711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A267711: Numbers k such that k mod 3 = k mod 5.
   The sequence consists of all integers congruent to 0, 1, or 2 modulo 15.
   Formula used:
     a(n) = 15 * floor((n-1)/3) + ((n-1) mod 3)
   for n ≥ 1, with offset 1 (a(1) = 0). */
seq_t a(idx_t n) {
    /* n is guaranteed to be ≥ 1 in valid calls. */
    idx_t q = (n - 1) / 3;          /* quotient floor((n-1)/3) */
    idx_t r = (n - 1) % 3;          /* remainder (n-1) mod 3 */
    return ((seq_t)15 * q) + r;     /* a(n) = 15*q + r */
}
