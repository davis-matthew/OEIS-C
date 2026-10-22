/*
 * A368771  a(n) = (n!)^3 * Sum_{k=0..n} k^2/(k!)^3.
 * https://oeis.org/A368771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = n^3 * a(n-1) + n^2 (n > 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;                     /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t si   = (seq_t)i;          /* cast index to seq_t */
        seq_t sq   = si * si;           /* i^2   */
        seq_t cube = sq * si;           /* i^3   */
        res = cube * res + sq;          /* recurrence */
    }
    return res;
}
