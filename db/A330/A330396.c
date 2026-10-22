/*
 * A330396  Permutation of the nonnegative integers partitioned into triples
 *          [3*k+2, 3*k+1, 3*k] for k >= 0.
 * https://oeis.org/A330396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 2 - 2*(n mod 3)
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    return ((seq_t)n + 2) - (2 * (seq_t)r);
}
