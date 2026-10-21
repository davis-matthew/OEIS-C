/*
 * A258717  If n even then 2*n^2-4*n else 2*n^2-4*n-3.
 * https://oeis.org/A258717
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2-4*n if n even * a(n) = 2*n^2-4*n-3 if n odd *
 *            Offset: 2 (a(2) is the first listed term)
 * Range:     u8 n <= 12, u16 n <= 182, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;               /* behaviour below the offset is irrelevant */

    seq_t nn = (seq_t)n;
    seq_t val = 2 * nn * nn - 4 * nn;
    if (n & 1)                  /* n is odd */
        val -= 3;
    return val;
}
