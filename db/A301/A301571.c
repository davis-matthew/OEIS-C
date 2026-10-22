/*
 * A301571  Number of vertices at distance 2 from a given vertex in the
 *          n-Keller graph.
 * https://oeis.org/A301571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + n - 1 for n > 1; a(1) = 0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 *= (seq_t)3;
    return pow3 + ((seq_t)n - 1);
}
