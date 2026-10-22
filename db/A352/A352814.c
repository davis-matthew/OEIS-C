/*
 * A352814  Solution to Forest of Numbers (Bosque de Números) puzzle if we
 *          start with the numbers 1 through n for an n X n square grid (see
 *          Comments).
 * https://oeis.org/A352814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n*(n+1)/2 + 2*floor((n-1)/2)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* convert n to seq_t */
    seq_t tri = nn * (nn + 1) / 2;       /* triangular number n(n+1)/2 */
    seq_t add = 2 * ((seq_t)(n - 1) / 2);/* 2*floor((n-1)/2) */
    return tri + add;
}
