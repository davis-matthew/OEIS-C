/*
 * A291368  The arithmetic function u(n,3,6).
 * https://oeis.org/A291368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest value among {3,4,5,7,11,13} that divides n;
 *            if no such value divides n, return 13
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 3 == 0) return (seq_t)3;
    if (n % 4 == 0) return (seq_t)4;
    if (n % 5 == 0) return (seq_t)5;
    if (n % 7 == 0) return (seq_t)7;
    if (n % 11 == 0) return (seq_t)11;
    return (seq_t)13;
}
