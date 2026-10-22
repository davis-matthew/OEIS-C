/*
 * A353737  Length of longest n-digit optimal prime ladder (base 10).
 * https://oeis.org/A353737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = 2*n + 1 for n ≥ 3, with a(1) = 2, a(2) = 4
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    return ((seq_t)n) * 2 + (seq_t)1;
}
