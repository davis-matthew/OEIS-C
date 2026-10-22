/*
 * A376951  Characteristic polynomial of the Pappus graph: a(n) =
 *          (n-3)*n^4*(n+3)*(n^2-3)^6.
 * https://oeis.org/A376951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n-3)*n^4*(n+3)*(n^2-3)^6
 * Range:     i8 n <= 0, i16 n <= 3, i32 n <= 3, i64 n <= 11, i128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;               /* convert index to seq_t */

    /* key intermediates */
    seq_t n_minus_3 = sn - 3;
    seq_t n_plus_3  = sn + 3;

    seq_t n4 = sn * sn * sn * sn;         /* n^4 */
    seq_t n_sq_minus_3 = sn * sn - 3;     /* n^2-3 */

    /* (n^2-3)^6 via repeated multiplication */
    seq_t n6sq = 1;
    for (int i = 0; i < 6; ++i) {
        n6sq *= n_sq_minus_3;
    }

    return n_minus_3 * n4 * n_plus_3 * n6sq;
}
