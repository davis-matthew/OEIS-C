/*
 * A358655  a(n) is the number of distinct scalar products which can be formed
 *          by pairs of signed permutations (V, W) of [n].
 * https://oeis.org/A358655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n < 4: a(0)=1, a(1)=2, a(2)=7, a(3)=24 For n ≥ 4: a(n) =
 *            (2*n^3 + 3*n^2 + n + 3) / 3 (OEIS A358655)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1726, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small n cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)24;

    /* Compute the cubic formula for n >= 4. */
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;          /* n^2 */
    seq_t n3  = n2 * sn;          /* n^3 */
    seq_t num = 2 * n3 + 3 * n2 + sn + 3; /* numerator: 2n^3 + 3n^2 + n + 3 */
    seq_t res = num / 3;          /* divide by 3 (exact for n >= 4) */
    return res;
}
