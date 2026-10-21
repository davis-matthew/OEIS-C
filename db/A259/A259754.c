/*
 * A259754  Numbers that are congruent to {3,9,15,18,21} mod 24.
 * https://oeis.org/A259754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24 * floor((n-1)/5) + {3,9,15,18,21}[(n-1) % 5]
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1 */
    idx_t idx   = n - 1;
    seq_t k     = (seq_t)(idx / 5);
    static const seq_t residues[5] = {3, 9, 15, 18, 21};
    seq_t r     = residues[idx % 5];
    return (seq_t)24 * k + r;
}
