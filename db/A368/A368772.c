/*
 * A368772  a(n) = (n!)^3 * Sum_{k=0..n} (k/k!)^3.
 * https://oeis.org/A368772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(n) = n^3*a(n-1)+n^3 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t cube = (seq_t)i * i * i;
        result = cube * (result + 1);
    }
    return result;
}
