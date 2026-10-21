/*
 * A292345  The second Zagreb index of the Aztec diamond AZ(n) (see the
 *          Ramanes et al. reference, Theorem 2.2).
 * https://oeis.org/A292345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64*n^2 + 64*n - 32
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)64 * nn * nn + (seq_t)64 * nn - (seq_t)32;
}
