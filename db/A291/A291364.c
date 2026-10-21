/*
 * A291364  The arithmetic function u(n,3,3).
 * https://oeis.org/A291364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 if n is divisible by 3 4 if n is divisible by 4 5 if n
 *            is divisible by 5 7 otherwise (Offset 1)
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
    return (seq_t)7;
}
