/*
 * A266393  Number of permutations of n letters that contain exactly 3
 *          distinguishable A's, 2 distinguishable B's and n-5 distinguishable
 *          other letters, where no A's are adjacent and no B's are adjacent.
 * https://oeis.org/A266393
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3)! * (n^3 - 11*n^2 + 44*n - 64)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) return 0;                 /* undefined below OEIS offset */

    /* compute (n-3)! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n - 3; ++i)
        fact *= (seq_t)i;

    /* compute the polynomial factor: n^3 - 11*n^2 + 44*n - 64 */
    seq_t sn   = (seq_t)n;
    seq_t poly = sn * sn * sn;           /* n^3 */
    poly -= (seq_t)11 * sn * sn;         /* -11*n^2 */
    poly += (seq_t)44 * sn;              /* +44*n */
    poly -= (seq_t)64;                   /* -64 */

    return fact * poly;
}
