/*
 * A302406  Total domination number of the n X n torus grid graph.
 * https://oeis.org/A302406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { n^2/4 if n mod 4 == 0 (n^2 + n + 2)/4 if n mod 4 == 1
 *            n^2/4 + 1 if n mod 4 == 2 (n^2 + n)/4 if n mod 4 == 3 }
 * Range:     u8 n <= 15, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;
    idx_t r  = n & 3;          /* n mod 4 */
    if (r == 0)
        return sq / 4;
    else if (r == 1)
        return (sq + (seq_t)n + (seq_t)2) / 4;
    else if (r == 2)
        return sq / 4 + (seq_t)1;
    else /* r == 3 */
        return (sq + (seq_t)n) / 4;
}
