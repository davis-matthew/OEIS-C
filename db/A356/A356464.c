/*
 * A356464  Number of black keys in each group of black keys on a standard
 *          88-key piano (left to right).
 * https://oeis.org/A356464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(1) = 1; for n ≥ 2, a(n) = 2 if n is even, else 3
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    return (n & 1) ? (seq_t)3 : (seq_t)2;
}
