/*
 * A272361  Numbers n such that (2^n + 1) / gcd(n, 2^n + 1) is not squarefree.
 * https://oeis.org/A272361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 182 * (2*n - 1)
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; n ≥ 1 */
    seq_t two_n_minus1 = ((seq_t)2 * (seq_t)n) - (seq_t)1;
    return (seq_t)182 * two_n_minus1;
}
