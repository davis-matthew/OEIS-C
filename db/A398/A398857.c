/*
 * A398857  Upper (2/3, 1)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((2/3) * T(n) + P(n)) where T(n) = n*(n+1)/2
 *            (triangular numbers) P(n) = n*(3*n-1)/2 (pentagonal numbers)
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;

    /* T(n) = n*(n+1)/2 */
    seq_t T = (sn * (sn + 1)) / 2;

    /* P(n) = n*(3*n-1)/2 */
    seq_t P = (sn * (3 * sn - 1)) / 2;

    /* V = 2*T + 3*P + 2  */
    seq_t V = 2 * T + 3 * P + 2;

    /* a(n) = ceil(V/3) = (V + 2) / 3, but
       for an integer V we can write directly V/3 after adding 2.
       Here V already contains the +2, so just divide by 3. */
    return V / 3;
}
