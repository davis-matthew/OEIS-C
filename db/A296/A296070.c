/*
 * A296070  Partial sums of A296069.
 * https://oeis.org/A296070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + (-1)^n)*n/2 for n>2; a(1) = 0, a(2) = 2
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n & 1) return (seq_t)n;      /* odd n: a(n) = n */
    return ((seq_t)n) << 1;          /* even n: a(n) = 2*n */
}
