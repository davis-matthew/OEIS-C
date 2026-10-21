/*
 * A278375  Edge-distinguishing chromatic number of ladder graph with 2n
 *          vertices.
 * https://oeis.org/A278375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = floor((n-1)/3) + k * where k = 1 if n==1, * 3 if n==2, *
 *            4 if 3 <= n <= 5, * 5 if n >= 6
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    seq_t base = ((seq_t)(n - 1)) / 3;
    if (n <= 5)
        base += (seq_t)4;
    else
        base += (seq_t)5;
    return base;
}
