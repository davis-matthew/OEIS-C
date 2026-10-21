/*
 * A284965  a(n) is the number of self-conjugate partitions of n which
 *          represent Chomp positions with Sprague-Grundy value 1.
 * https://oeis.org/A284965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n odd or n < 6; otherwise a(n) = n/4
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the fact that for even n ≥ 6 the count of
   self‑conjugate partitions of n that are Chomp positions with
   Sprague–Grundy value 1 equals ⌊n/4⌋. */

seq_t a(idx_t n)
{
    /* n < 6 handles n = 2, 4 and the initial zeros. */
    if (n < 6 || (n & 1))   /* odd n or n < 6 */
        return 0;
    return (seq_t)(n / 4);
}
