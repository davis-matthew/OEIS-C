/*
 * A274406  Numbers m such that 9 divides m*(m + 1).
 * https://oeis.org/A274406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = (18*n + 7*(-1)^n - 11)/4
   Equivalent simple formula:
   if n is odd:   a(n) = 9 * floor(n/2)
   if n is even:  a(n) = 9 * floor(n/2) - 1
   (All arithmetic performed in seq_t) */
seq_t a(idx_t n)
{
    idx_t m = n >> 1;          /* floor(n/2) */
    if (n & 1)                 /* odd n */
        return (seq_t)9 * (seq_t)m;
    else                       /* even n */
        return (seq_t)9 * (seq_t)m - (seq_t)1;
}
