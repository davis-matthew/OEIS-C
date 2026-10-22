/*
 * A303299  Generalized 22-gonal (or icosidigonal) numbers: m*(10*m - 9) with
 *          m = 0, +1, -1, +2, -2, +3, -3, ...
 * https://oeis.org/A303299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^2 + 9*n)/2 for even n; a(n) = (5*n^2 + n - 4)/2 for
 *            odd n
 * Range:     u8 n <= 9, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;
    if ((n & 1) == 0) {
        /* even n */
        return (((seq_t)5 * nt * nt) + ((seq_t)9 * nt)) / 2;
    } else {
        /* odd n */
        return (((seq_t)5 * nt * nt) + nt - (seq_t)4) / 2;
    }
}
