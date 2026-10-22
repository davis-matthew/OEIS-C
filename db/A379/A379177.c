/*
 * A379177  Lower matching number of the n X n X n grid graph.
 * https://oeis.org/A379177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(n) = ceil(n^3 / 3) + floor((n - 2) / 3) (for n ≥ 2, a(1) = 0)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t nn  = (seq_t)n;
    seq_t n3  = nn * nn * nn;
    seq_t t1  = (n3 + 2) / 3;   /* ceil(n^3 / 3) */
    seq_t t2  = (n - 2) / 3;    /* floor((n - 2) / 3) */
    return t1 + t2;
}
