/*
 * A354673  Smallest number of unit cells that must be removed from an n X n
 *          square board in order to avoid any cycles.
 * https://oeis.org/A354673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^2/3 - n/6 + 4/3) - ceil(n/2) Simplified
 *            computation: a(n) = ((2*n*n - n + 8) + 5)/6 - (n + 1)/2
 * Range:     u8 n <= 11, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 1;

    seq_t N = (seq_t)n;
    seq_t A = 2 * N * N - N + 8;          /* numerator of first ceil */
    seq_t term1 = (A + 5) / 6;            /* ceil(A/6) */
    seq_t term2 = (N + 1) / 2;            /* ceil(n/2) */
    return term1 - term2;
}
