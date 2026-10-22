/*
 * A399563  Numbers k such that the tens digit of k^2 equals the units digit
 *          of 2*k.
 * https://oeis.org/A399563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less, new
 * Formula:   a(n) = floor((n-1)/11)*50 + residues[(n-1)%11]; residues = {0,
 *            7, 8, 9, 10, 11, 12, 13, 20, 30, 40}
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t residues[11] = {0, 7, 8, 9, 10, 11, 12, 13, 20, 30, 40};

    idx_t m      = n - (idx_t)1;       /* zero‑based index of the term */
    idx_t block  = m / (idx_t)11;      /* count of completed 50‑blocks */
    idx_t off    = m % (idx_t)11;      /* position within the block   */

    seq_t term = (seq_t)block * (seq_t)50 + residues[(unsigned)off];
    return term;
}
