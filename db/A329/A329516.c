/*
 * A329516  Expansion of (x^4 - x^3 - 3*x^2 - 2*x - 1)/(x - 1).
 * https://oeis.org/A329516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=6, a(3)=7, a(n)=6 for all n>=4
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)3;
    if (n == 2u) return (seq_t)6;
    if (n == 3u) return (seq_t)7;
    return (seq_t)6;
}
