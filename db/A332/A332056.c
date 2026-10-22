/*
 * A332056  a(1) = 1, then a(n+1) = a(n) - (-1)^a(n) Sum_{k=1..n} a(k): if
 *          a(n) is odd, add the partial sum, else subtract.
 * https://oeis.org/A332056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(3k-2) = 1, a(3k-1) = 2k, a(3k) = 1-2k (k≥1)
 * Range:     i8 n <= 190, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t rem  = n % 3;
    idx_t k0   = (n + 2) / 3;          /* k = (n+2)/3 works for all residues */
    seq_t k    = (seq_t)k0;

    if (rem == 1) {
        return 1;
    }
    if (rem == 2) {
        return 2 * k;
    }
    /* rem == 0 */
    return 1 - 2 * k;
}
