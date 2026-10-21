/*
 * A259750  Numbers that are congruent to {14, 22} mod 24.
 * https://oeis.org/A259750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 5461, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers congruent to {14, 22} modulo 24
   Formula: a(n) = 12*n + 2 for odd n, a(n) = 12*n - 2 for even n   */
seq_t a(idx_t n)
{
    if (n & 1)          /* odd n */
        return (seq_t)(24 * ((n - 1) / 2) + 14);
    else                /* even n */
        return (seq_t)(24 * ((n / 2) - 1) + 22);
}
